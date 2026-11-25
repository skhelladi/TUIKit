#include "tuikit/widgets/TUIFileDialog.h"
#include "tuikit/core/TUIIcons.h"
#include "tuikit/core/TUIStyle.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

namespace TUIKIT {

using namespace ftxui; // NOLINT

namespace {

namespace fs = std::filesystem;

std::string toLower(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return result;
}

/// Expand a leading "~" to the user's home directory.
std::string expandTilde(const std::string& path) {
    if (path.empty() || path[0] != '~') return path;
    const char* home = std::getenv("HOME");
    if (!home) return path;
    if (path.size() == 1) return home;
    if (path[1] == '/') return std::string(home) + path.substr(1);
    return path;
}

/// Minimal wildcard match supporting '*' and '?' (e.g. "*.json").
bool wildcardMatch(const std::string& name, const std::string& pattern) {
    const std::string n = toLower(name);
    const std::string p = toLower(pattern);
    std::size_t i = 0, j = 0, star = std::string::npos, mark = 0;
    while (i < n.size()) {
        if (j < p.size() && (p[j] == '?' || p[j] == n[i])) {
            ++i; ++j;
        } else if (j < p.size() && p[j] == '*') {
            star = j++;
            mark = i;
        } else if (star != std::string::npos) {
            j = star + 1;
            i = ++mark;
        } else {
            return false;
        }
    }
    while (j < p.size() && p[j] == '*') ++j;
    return j == p.size();
}

} // namespace

// -----------------------------------------------------------------------------
// FileDialogImpl
// -----------------------------------------------------------------------------

class TUIFileDialog::FileDialogImpl : public std::enable_shared_from_this<FileDialogImpl> {
public:
    FileDialogImpl(TUIApp* app, std::string start_dir, std::vector<std::string> filters,
                   FileDialogMode mode, TUIFileDialog::Callback callback)
        : app_(app), filters_(std::move(filters)), mode_(mode), callback_(std::move(callback)) {
        std::error_code ec;
        fs::path dir = fs::absolute(expandTilde(start_dir), ec);
        // Fall back to the working directory when the start dir is unusable.
        if (ec || dir.empty() || !fs::is_directory(dir, ec)) {
            dir = fs::current_path(ec);
            if (ec) dir = fs::path(".");
        }
        current_dir_ = dir;
    }

    /// Must be called once the impl is owned by a shared_ptr.
    void init() {
        build();
        refresh();
    }

    void open() {
        app_->show_modal(component_);
    }

    std::string selectedEntry() const {
        if (selected_ < 0 || selected_ >= static_cast<int>(entries_.size())) {
            return "";
        }
        return entryPath(selected_).string();
    }

private:
    struct Entry {
        std::string name;
        bool is_dir = false;
    };

    // --- construction -------------------------------------------------------

    void build() {
        InputOption input_option;
        input_option.multiline = false;
        input_ = Input(&path_input_, "", input_option);
        ok_button_ = Button("OK", [self = shared_from_this()] { self->accept(); });
        cancel_button_ = Button("Cancel", [self = shared_from_this()] { self->cancel(); });
        buttons_ = Container::Horizontal({ok_button_, cancel_button_});

        MenuOption menu_option;
        menu_option.on_change = [self = shared_from_this()] { self->updatePathFromSelection(); };
        menu_option.on_enter = [self = shared_from_this()] { self->activateEntry(); };
        menu_ = Menu(&labels_, &selected_, menu_option);

        container_ = Container::Vertical({menu_, input_, buttons_});
        container_->SetActiveChild(menu_.get());

        component_ = Renderer(container_, [self = shared_from_this()] { return self->render(); });
        component_ = CatchEvent(component_, [self = shared_from_this()](Event event) {
            return self->handleEvent(event);
        });
    }

    // --- directory listing ----------------------------------------------------

    bool matchesFilters(const std::string& name) const {
        if (filters_.empty()) return true;
        for (const auto& pattern : filters_) {
            if (wildcardMatch(name, pattern)) return true;
        }
        return false;
    }

    void refresh() {
        entries_.clear();
        labels_.clear();
        std::error_code ec;
        const bool has_parent = current_dir_.parent_path() != current_dir_;
        if (has_parent) {
            entries_.push_back({"..", true});
        }
        for (fs::directory_iterator it(current_dir_, ec), end; !ec && it != end; it.increment(ec)) {
            const std::string name = it->path().filename().string();
            if (name.empty()) continue;
            const bool hidden = name[0] == '.';
            if (hidden && !show_hidden_) continue;
            std::error_code dir_ec;
            const bool is_dir = it->is_directory(dir_ec);
            if (mode_ == FileDialogMode::SelectDirectory && !is_dir) {
                continue; // Only directories are relevant in directory mode.
            }
            if (!is_dir && !matchesFilters(name)) {
                continue;
            }
            entries_.push_back({name, is_dir});
        }
        std::sort(entries_.begin() + (has_parent ? 1 : 0), entries_.end(),
                  [](const Entry& a, const Entry& b) {
                      if (a.is_dir != b.is_dir) return a.is_dir; // directories first
                      return toLower(a.name) < toLower(b.name);
                  });

        // Selectable display labels: "📁 name/" for directories, "📄 name" for files.
        for (const auto& entry : entries_) {
            if (entry.is_dir) {
                labels_.push_back(ICON::Open + " " + entry.name + "/");
            } else {
                labels_.push_back(ICON::NewFile + " " + entry.name);
            }
        }

        selected_ = 0;
        error_.clear();
        path_input_ = current_dir_.string() + "/";
    }

    fs::path entryPath(int index) const {
        const Entry& entry = entries_[index];
        if (entry.name == "..") {
            return current_dir_.parent_path();
        }
        return current_dir_ / entry.name;
    }

    // --- navigation and selection ---------------------------------------------

    void goUp() {
        const fs::path parent = current_dir_.parent_path();
        if (parent == current_dir_) return;
        current_dir_ = parent.empty() ? fs::path("/") : parent;
        refresh();
    }

    void updatePathFromSelection() {
        if (input_focused_ || selected_ < 0 || selected_ >= static_cast<int>(entries_.size())) {
            return;
        }
        path_input_ = entryPath(selected_).string();
    }

    void activateEntry() {
        if (selected_ < 0 || selected_ >= static_cast<int>(entries_.size())) {
            accept();
            return;
        }
        const Entry& entry = entries_[selected_];
        if (entry.name == "..") {
            goUp();
            return;
        }
        if (entry.is_dir) {
            enterSelectedDirectory();
            return;
        }
        accept();
    }

    /// In SelectDirectory mode Enter selects the highlighted directory;
    /// otherwise (and via the dedicated Right key) it navigates into it.
    void enterSelectedDirectory() {
        if (selected_ < 0 || selected_ >= static_cast<int>(entries_.size())) {
            return;
        }
        const Entry& entry = entries_[selected_];
        if (entry.name != ".." && entry.is_dir && mode_ == FileDialogMode::SelectDirectory) {
            finish(current_dir_ / entry.name);
            return;
        }
        current_dir_ /= entry.name;
        refresh();
    }

    // --- validation -------------------------------------------------------------

    void finish(const fs::path& path) {
        if (callback_) {
            callback_(fs::absolute(path).lexically_normal().string());
        }
        app_->close_modal();
    }

    void accept() {
        std::error_code ec;
        std::string raw_path = expandTilde(path_input_);
        fs::path p(raw_path);
        if (p.is_relative()) {
            p = current_dir_ / p;
        }

        if (mode_ == FileDialogMode::SelectDirectory) {
            if (fs::is_directory(p, ec)) {
                finish(p);
            } else {
                error_ = "Not a directory: " + p.string();
            }
            return;
        }
        if (fs::is_directory(p, ec)) {
            current_dir_ = p;
            refresh();
            return;
        }
        if (mode_ == FileDialogMode::OpenFile) {
            if (fs::exists(p, ec) && fs::is_regular_file(p, ec)) {
                finish(p);
            } else {
                error_ = "File does not exist: " + p.string();
            }
            return;
        }
        // SaveFile: confirm before overwriting an existing file.
        if (fs::exists(p, ec)) {
            auto self = shared_from_this();
            app_->confirm("Overwrite existing file?\n" + p.string(),
                          [self, p](bool confirmed) {
                              if (confirmed) self->finish(p);
                          },
                          "Confirm Overwrite");
            return;
        }
        finish(p);
    }

    void cancel() {
        if (callback_) {
            callback_("");
        }
        app_->close_modal();
    }

    // --- events -------------------------------------------------------------------

    bool handleEvent(Event event) {
        // Mouse events: the menu, the input and the buttons handle their own
        // clicks and wheel scrolling.
        if (event.is_mouse()) {
            return false;
        }

        if (input_focused_) {
            if (event == Event::Return) {
                accept();
                return true;
            }
            if (event == Event::Escape) {
                cancel();
                return true;
            }
            if (event == Event::Tab || event == Event::TabReverse) {
                setInputFocused(false);
                return true;
            }
            return input_->OnEvent(event);
        }

        // List navigation mode. Input is not focus-gated in this FTXUI version
        // and Container dispatches to every child, so unhandled keys must be
        // swallowed here to keep the path field clean.
        if (event == Event::Escape) {
            cancel();
            return true;
        }
        if (event == Event::Tab || event == Event::TabReverse) {
            setInputFocused(true);
            return true;
        }
        if (event == Event::Backspace) {
            goUp();
            return true;
        }
        if (event == Event::ArrowRight) {
            // Dedicated "navigate into directory" key (Enter selects in
            // SelectDirectory mode instead of navigating).
            enterSelectedDirectory();
            return true;
        }
        if (event == Event::Return) {
            activateEntry();
            return true;
        }
        if (event == Event::Character("h") || event == Event::Character("H")) {
            show_hidden_ = !show_hidden_;
            refresh();
            return true;
        }
        if (event.is_character()) {
            return true; // Swallow stray typing (do not leak into the path input).
        }
        // ArrowUp/Down, PageUp/Down, Home/End fall through to the menu list.
        return false;
    }

    void setInputFocused(bool focused) {
        input_focused_ = focused;
        container_->SetActiveChild(focused ? input_.get() : menu_.get());
    }

    // --- rendering -----------------------------------------------------------------

    Element render() {
        auto& theme = TUIStyle::instance().currentTheme();

        const char* title = (mode_ == FileDialogMode::OpenFile)   ? " Open File "
                          : (mode_ == FileDialogMode::SaveFile)   ? " Save File "
                                                                  : " Select Directory ";
        std::string filters_text;
        for (const auto& f : filters_) {
            if (!filters_text.empty()) filters_text += " ";
            filters_text += f;
        }

        Element list_element;
        if (labels_.empty()) {
            list_element = text("(empty directory)") | dim;
        } else {
            list_element = menu_->Render() | flex;
        }

        Elements dialog_rows = {
            text(title) | bold | color(theme.primary) | center,
            separator(),
            text(current_dir_.string()) | color(theme.secondary),
            list_element,
            separator(),
            hbox({text("Path: "), input_->Render() | flex}),
        };
        if (!filters_text.empty()) {
            dialog_rows.push_back(text("Filters: " + filters_text) | dim);
        }
        dialog_rows.push_back(separator());
        dialog_rows.push_back(buttons_->Render() | center);
        if (!error_.empty()) {
            dialog_rows.push_back(text(error_) | color(theme.error));
        }
        dialog_rows.push_back(text("Enter: select  Right: open dir  Backspace: parent  Tab: edit path  h: hidden  Esc: cancel") | dim);

        return vbox(std::move(dialog_rows)) | border | size(WIDTH, LESS_THAN, 80);
    }

    TUIApp* app_;
    std::vector<std::string> filters_;
    FileDialogMode mode_;
    TUIFileDialog::Callback callback_;

    fs::path current_dir_;
    std::vector<Entry> entries_;
    std::vector<std::string> labels_; // Selectable display labels, parallel to entries_.
    int selected_ = 0;
    bool show_hidden_ = false;
    bool input_focused_ = false;
    std::string path_input_;
    std::string error_;

    ftxui::Component menu_;
    ftxui::Component input_;
    ftxui::Component ok_button_;
    ftxui::Component cancel_button_;
    ftxui::Component buttons_;
    ftxui::Component container_;
    ftxui::Component component_;
};

// -----------------------------------------------------------------------------
// TUIFileDialog public API
// -----------------------------------------------------------------------------

TUIFileDialog::TUIFileDialog(TUIApp& app, const std::string& start_dir,
                             const std::vector<std::string>& filters,
                             FileDialogMode mode)
    : impl_(std::make_shared<FileDialogImpl>(&app, start_dir, filters, mode, nullptr)) {
    impl_->init();
}

TUIFileDialog::~TUIFileDialog() = default;

void TUIFileDialog::open() {
    impl_->open();
}

std::string TUIFileDialog::selectedEntry() const {
    return impl_->selectedEntry();
}

void TUIFileDialog::pickFile(TUIApp& app, const std::string& start_dir,
                             const std::vector<std::string>& filters,
                             FileDialogMode mode, Callback callback) {
    auto impl = std::make_shared<FileDialogImpl>(&app, start_dir, filters, mode, std::move(callback));
    impl->init();
    impl->open();
    // Lifetime: the modal component's handlers capture shared_from_this(), so
    // TUIApp keeps the impl alive until close_modal() drops the component.
}

} // namespace TUIKIT
