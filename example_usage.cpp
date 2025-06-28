// =============================================================================
// TUIKIT FRAMEWORK EXAMPLE APPLICATION (v4)
// =============================================================================

#include "tuikit_framework.h"
#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <thread>
#include <chrono>
#include <atomic>
#include <filesystem>         // For file system operations
#include <mutex>              // For std::mutex
#include <condition_variable> // For std::condition_variable

using namespace TUIKIT;

// =============================================================================
// APPLICATION PAGES
// =============================================================================

// Home Page
class HomePage
{
public:
    Component get_component()
    {
        return Renderer([]
                        { return vbox(std::vector<Element>{
                                     text("🚀 Welcome to TUIKIT Framework") | bold | center | color(Color::Cyan),
                                     text("") | center,
                                     text("A modern framework for creating rich") | center,
                                     text("command-line applications.") | center,
                                     text("") | center,
                                     text("👈 Use the menu on the left to explore examples") | center | color(Color::Green),
                                 }) |
                                 border | center; });
    }
};

// Form Page
class FormPage
{
public:
    FormPage(Application &app) : app_(app)
    {
        name_input_ = std::make_shared<TUIKInput>("Enter your name", "Name:");
        email_input_ = std::make_shared<TUIKInput>("exemple@email.com", "Email:");
        email_input_->set_validator([](const std::string &email)
                                    { return email.find('@') != std::string::npos && email.find('.') != std::string::npos; });

        submit_button_ = WidgetBuilder::create_button("Validate", [this]
                                                      {
            if (!email_input_->is_valid()) {
                app_.show_error("The email address is not valid.");
                return;
            }
            std::string msg = "Name: " + name_input_->get_text() + "\nEmail: " + email_input_->get_text();            app_.show_message(msg, "Registration successful"); });

        main_panel_ = std::make_shared<TUIKPanel>("Registration Form");
        main_panel_->add_child(name_input_->get_component());
        main_panel_->add_child(email_input_->get_component());
        main_panel_->add_child(submit_button_->get_component());
    }

    Component get_component()
    {
        return main_panel_->get_component();
    }

private:
    Application &app_;
    std::shared_ptr<TUIKInput> name_input_;
    std::shared_ptr<TUIKInput> email_input_;
    std::shared_ptr<TUIKButton> submit_button_;
    std::shared_ptr<TUIKPanel> main_panel_;
};
// ===============================
// TASK MANAGER PAGE
// ===============================
class TaskManagerPage
{
public:
    TaskManagerPage(Application &app) : app_(app)
    {
        tasks_ = {
            {"Finish TUIKIT project", false, 3, nullptr},
            {"Review code", false, 2, nullptr},
            {"Write documentation", false, 1, nullptr},
            {"Test application", true, 2, nullptr}};
        update_task_list();
        task_menu_ = std::make_shared<StyledMenu>(task_display_);
        new_task_input_ = std::make_shared<TUIKInput>("New task...", "Task:");
        priority_radio_ = std::make_shared<StyledRadiobox>(
            std::vector<std::string>{"Low", "Medium", "High"}, 1);
        add_btn_ = WidgetBuilder::create_button("Add", [this]
                                                { add_task(); });
        toggle_btn_ = WidgetBuilder::create_button("Toggle", [this]
                                                   { toggle_task(); });
        delete_btn_ = WidgetBuilder::create_button("Delete", [this]
                                                   { delete_task(); });

        // Main layout
        auto left_panel = Container::Vertical({Renderer([]
                                                        { return text("Task List") | bold | color(Color::Cyan); }),
                                               Renderer([this]
                                                        { return task_menu_->render(); })});

        auto right_panel = Container::Vertical({Renderer([]
                                                         { return text("New Task") | bold | color(Color::Green); }),
                                                Renderer([this]
                                                         { return new_task_input_->render(); }),
                                                Renderer([]
                                                         { return text("Priority:") | color(Color::White); }),
                                                Renderer([this]
                                                         { return priority_radio_->render(); }),
                                                add_btn_->get_component()});

        auto bottom_panel = Container::Horizontal({toggle_btn_->get_component(),
                                                   delete_btn_->get_component()});

        main_panel_ = std::make_shared<TUIKPanel>("Task Manager");
        main_panel_->add_child(
            Renderer([left_panel, right_panel, bottom_panel]() -> Element
                     { return vbox({hbox({left_panel->Render() | flex,
                                          right_panel->Render() | size(WIDTH, EQUAL, 30)}) |
                                        flex,
                                    bottom_panel->Render()}); }));
    }

    Component get_component()
    {
        return main_panel_->get_component();
    }

private:
    struct Task
    {
        std::string name;
        bool completed = false;
        int priority = 1;
        std::shared_ptr<TaskManagerPage> task_manager_page_;
    };
    Application &app_;
    std::vector<Task> tasks_;
    std::vector<std::string> task_display_;
    std::shared_ptr<StyledMenu> task_menu_;
    std::shared_ptr<TUIKInput> new_task_input_;
    std::shared_ptr<StyledRadiobox> priority_radio_;
    std::shared_ptr<TUIKButton> add_btn_, toggle_btn_, delete_btn_;
    std::shared_ptr<TUIKPanel> main_panel_;

    void update_task_list()
    {
        task_display_.clear();
        for (const auto &task : tasks_)
        {
            std::string status = task.completed ? "✓" : "○";
            std::string priority_str = task.priority == 3 ? "!" : task.priority == 2 ? "·"
                                                                                     : " ";
            task_display_.push_back(status + " " + priority_str + " " + task.name);
        }
        if (task_menu_)
            task_menu_->get_component();
    }

    void add_task()
    {
        std::string task_name = new_task_input_->get_text();
        if (task_name.empty())
        {
            app_.show_message("Please enter a task name");
            return;
        }
        Task new_task;
        new_task.name = task_name;
        new_task.priority = priority_radio_->get_selected() + 1;
        new_task.completed = false;
        tasks_.push_back(new_task);
        update_task_list();
        new_task_input_->set_text("");
        app_.show_message("Task added successfully");
        app_.request_redraw();
    }

    void toggle_task()
    {
        int selected = task_menu_->get_selected();
        if (selected >= 0 && selected < static_cast<int>(tasks_.size()))
        {
            tasks_[selected].completed = !tasks_[selected].completed;
            update_task_list();
            app_.request_redraw();
        }
    }

    void delete_task()
    {
        int selected = task_menu_->get_selected();
        if (selected >= 0 && selected < static_cast<int>(tasks_.size()))
        {
            app_.confirm("Delete task '" + tasks_[selected].name + "'?", [this, selected](bool confirmed)
                         {
                if (confirmed) {
                    tasks_.erase(tasks_.begin() + selected);
                    update_task_list();
                    app_.request_redraw();
                } });
        }
    }
};
// ===============================
// CALCULATOR PAGE
// ===============================
class CalculatorPage
{
public:
    CalculatorPage(Application &app) : app_(app), result_(0.0), operation_(' '), operand_(0.0)
    {
        display_text_ = "0";
        create_interface();
    }

    Component get_component() { return main_panel_; }

private:
    Application &app_;
    double result_;
    char operation_;
    double operand_;
    std::string display_text_;
    Component main_panel_;

    void create_interface()
    {
        std::vector<Component> number_buttons;
        for (int i = 0; i <= 9; ++i)
        {
            number_buttons.push_back(Button(std::to_string(i), [this, i]
                                            { input_number(i); app_.request_redraw(); }));
        }
        auto add_btn = Button("+", [this]
                              { set_operation('+'); app_.request_redraw(); });
        auto sub_btn = Button("-", [this]
                              { set_operation('-'); app_.request_redraw(); });
        auto mul_btn = Button("×", [this]
                              { set_operation('*'); app_.request_redraw(); });
        auto div_btn = Button("÷", [this]
                              { set_operation('/'); app_.request_redraw(); });
        auto equals_btn = Button("=", [this]
                                 { calculate(); app_.request_redraw(); });
        auto clear_btn = Button("C", [this]
                                { clear(); app_.request_redraw(); });
        auto decimal_btn = Button(".", [this]
                                  { input_decimal(); app_.request_redraw(); });

        auto display = Renderer([this]
                                { return vbox({text(display_text_) | size(HEIGHT, EQUAL, 1) | size(WIDTH, EQUAL, 20) |
                                               center | border | color(Color::Green)}); });

        auto row1 = Container::Horizontal({clear_btn, number_buttons[7], number_buttons[8], number_buttons[9], div_btn});
        auto row2 = Container::Horizontal({number_buttons[4], number_buttons[5], number_buttons[6], mul_btn});
        auto row3 = Container::Horizontal({number_buttons[1], number_buttons[2], number_buttons[3], sub_btn});
        auto row4 = Container::Horizontal({number_buttons[0], decimal_btn, equals_btn, add_btn});

        main_panel_ = Container::Vertical({display,
                                           row1, row2, row3, row4});
    }

    void input_number(int digit)
    {
        if (display_text_ == "0" || display_text_ == "Error")
        {
            display_text_ = std::to_string(digit);
        }
        else
        {
            display_text_ += std::to_string(digit);
        }
    }
    void input_decimal()
    {
        if (display_text_.find('.') == std::string::npos)
        {
            display_text_ += ".";
        }
    }
    void set_operation(char op)
    {
        try
        {
            result_ = std::stod(display_text_);
            operation_ = op;
            display_text_ = "0";
        }
        catch (...)
        {
            display_text_ = "Error";
        }
    }
    void calculate()
    {
        try
        {
            operand_ = std::stod(display_text_);
            switch (operation_)
            {
            case '+':
                result_ += operand_;
                break;
            case '-':
                result_ -= operand_;
                break;
            case '*':
                result_ *= operand_;
                break;
            case '/':
                if (operand_ == 0)
                {
                    display_text_ = "Error";
                    return;
                }
                result_ /= operand_;
                break;
            default:
                result_ = operand_;
                break;
            }
            display_text_ = std::to_string(result_);
            operation_ = ' ';
        }
        catch (...)
        {
            display_text_ = "Error";
        }
    }
    void clear()
    {
        result_ = 0.0;
        operation_ = ' ';
        operand_ = 0.0;
        display_text_ = "0";
    }
};

// ===============================
// SYSTEM MONITOR PAGE
// ===============================
class SystemMonitorPage
{
public:
    SystemMonitorPage(Application &app) : app_(app), cpu_usage_(0.0f), memory_usage_(0.0f), disk_usage_(0.0f), running_(true)
    {
        cpu_bar_ = std::make_shared<ProgressBar>(0.0f, "CPU:");
        memory_bar_ = std::make_shared<ProgressBar>(0.0f, "Memory:");
        disk_bar_ = std::make_shared<ProgressBar>(0.0f, "Disk:");

        system_info_ = "System: Linux | "
                       "Arch: x86_64 | "
                       "Cores: 8 (16 threads) | "
                       "RAM: 32 GB";

        process_list_ = {
            "PID   | Nom           | CPU% | Mem%",
            "------|---------------|------|-----",
            "1234  | chrome        | 15.2 | 8.4",
            "5678  | code          | 8.1  | 12.3",
            "9012  | firefox       | 12.5 | 15.7",
            "3456  | system        | 2.1  | 3.2"};

        process_menu_ = std::make_shared<StyledMenu>(process_list_);

        auto stats_panel = Container::Vertical({Renderer([]
                                                         { return text("System Statistics") | bold | color(Color::Green); }),
                                                Renderer([this]
                                                         { return cpu_bar_->render(); }),
                                                Renderer([this]
                                                         { return memory_bar_->render(); }),
                                                Renderer([this]
                                                         { return disk_bar_->render(); }),
                                                Renderer([this]
                                                         { return paragraph(system_info_) | color(Color::White); })});

        auto process_panel = Container::Vertical(std::vector<Component>{
            Renderer([]
                     { return text("Processes") | bold | color(Color::Blue); }),
            Renderer([this]
                     { return process_menu_->render(); }) |
                flex});

        main_panel_ = Container::Horizontal({stats_panel | size(WIDTH, EQUAL, 35),
                                             process_panel | flex});

        start_monitoring();
    }

    ~SystemMonitorPage()
    {
        stop_monitoring();
    }

    Component get_component() { return main_panel_; }

    void update_stats()
    {
        static int counter = 0;
        counter++;
        cpu_usage_ = 0.3f + 0.4f * std::sin(counter * 0.1f);
        memory_usage_ = 0.6f + 0.2f * std::cos(counter * 0.15f);
        disk_usage_ = 0.4f + 0.1f * std::sin(counter * 0.05f);
        cpu_bar_->set_value(cpu_usage_);
        memory_bar_->set_value(memory_usage_);
        disk_bar_->set_value(disk_usage_);
        app_.request_redraw();
    }

private:
    Application &app_;
    std::shared_ptr<ProgressBar> cpu_bar_;
    std::shared_ptr<ProgressBar> memory_bar_;
    std::shared_ptr<ProgressBar> disk_bar_;
    std::shared_ptr<StyledMenu> process_menu_;
    Component main_panel_;

    float cpu_usage_, memory_usage_, disk_usage_;
    std::string system_info_;
    std::vector<std::string> process_list_;

    std::thread monitor_thread_;
    std::atomic<bool> running_;

    void start_monitoring()
    {
        monitor_thread_ = std::thread([this]()
                                      {
            while (running_) {
                update_stats();
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            } });
    }

    void stop_monitoring()
    {
        running_ = false;
        if (monitor_thread_.joinable())
        {
            monitor_thread_.join();
        }
    }
};

// ===============================
// NEW COMPONENTS PAGE
// ===============================
class NewComponentsPage
{
public:
    NewComponentsPage(Application &app) : app_(app), slider_value_(0.5f)
    {
        // TUIKSlider Example
        slider_ = WidgetBuilder::create_slider("Volume", &slider_value_, 0.0f, 1.0f, 0.05f);

        // TUIKTextArea Example
        text_area_ = WidgetBuilder::create_text_area("Enter multi-line text here...", "Description:", 5);

        // TUIKTreeView Example
        TreeNode root_node;
        root_node.label = "Root";
        root_node.children.push_back({"Child 1", {}});
        root_node.children.push_back(TreeNode{"Child 2", {TreeNode{"Grandchild 2.1", {}}, TreeNode{"Grandchild 2.2", {}}}});
        root_node.children.push_back(TreeNode{"Child 3", {}});
        tree_view_ = WidgetBuilder::create_tree_view(root_node);

        // TUIKTabbedPane Example
        std::vector<std::string> tab_titles = {"Tab A", "Tab B", "Tab C"};
        std::vector<Component> tab_contents;
        tab_contents.push_back(Renderer([]
                                        { return text("Content of Tab A"); }));
        tab_contents.push_back(Renderer([]
                                        { return text("Content of Tab B"); }));
        tab_contents.push_back(Renderer([]
                                        { return text("Content of Tab C"); }));
        tabbed_pane_ = WidgetBuilder::create_tabbed_pane(tab_titles, tab_contents);

        // TUIKScrollableContainer Example
        auto long_text_content = vbox({
            text("This is a long text content that should be scrollable."),
            text("Line 1"),
            text("Line 2"),
            text("Line 3"),
            text("Line 4"),
            text("Line 5"),
            text("Line 6"),
            text("Line 7"),
            text("Line 8"),
            text("Line 9"),
            text("Line 10"),
            text("Line 11"),
            text("Line 12"),
            text("Line 13"),
            text("Line 14"),
            text("Line 15"),
        });
        scrollable_container_ = WidgetBuilder::create_scrollable_container(Renderer([long_text_content]
                                                                                    { return long_text_content; }),
                                                                           5);

        // Notification Example Button
        show_notification_button_ = WidgetBuilder::create_button("Show Notification", [this]
                                                                 { Notification::show(app_, "This is a test notification!", "New Message"); });

        main_panel_ = std::make_shared<TUIKPanel>("New Components Demo");
        main_panel_->add_child(Renderer([]
                                        { return text("TUIKSlider:"); }));
        main_panel_->add_child(slider_->get_component());
        main_panel_->add_child(Renderer([this]
                                        { return text("TUIKSlider Value: " + std::to_string(slider_value_)); }));
        main_panel_->add_child(Renderer([]
                                        { return text("TUIKTextArea:"); }));
        main_panel_->add_child(text_area_->get_component());
        main_panel_->add_child(Renderer([]
                                        { return text("TUIKTreeView:"); }));
        main_panel_->add_child(tree_view_->get_component());
        main_panel_->add_child(Renderer([]
                                        { return text("TUIKTabbedPane:"); }));
        main_panel_->add_child(tabbed_pane_->get_component());
        main_panel_->add_child(Renderer([]
                                        { return text("Scrollable Container:"); }));
        main_panel_->add_child(scrollable_container_->get_component());
        main_panel_->add_child(show_notification_button_->get_component());
    }

    Component get_component()
    {
        return main_panel_->get_component();
    }

private:
    Application &app_;
    float slider_value_;
    std::shared_ptr<TUIKSlider> slider_;
    std::shared_ptr<TUIKTextArea> text_area_;
    std::shared_ptr<TUIKTreeView> tree_view_;
    std::shared_ptr<TUIKTabbedPane> tabbed_pane_;
    std::shared_ptr<TUIKScrollableContainer> scrollable_container_;
    std::shared_ptr<TUIKButton> show_notification_button_;
    std::shared_ptr<TUIKPanel> main_panel_;
};

// ===============================
// FILE EXPLORER PAGE
// ===============================
// ===============================
// FILE EXPLORER PAGE
// ===============================
// ===============================
// FILE EXPLORER PAGE
// ===============================
class FileExplorerPage : public ftxui::ComponentBase, public std::enable_shared_from_this<FileExplorerPage>
{
public:
    FileExplorerPage(Application &app) : app_(app), running_(true), refresh_requested_(false), new_data_ready_(false)
    {
        current_path_ = std::filesystem::current_path();
        next_path_ = current_path_;

        fs_worker_thread_ = std::thread(&FileExplorerPage::fs_worker_loop, this);
        request_file_list_update();

        file_menu_ = std::make_shared<StyledMenu>(file_display_names_);

        up_btn_ = WidgetBuilder::create_button("↑ Parent", [this] {
            std::unique_lock<std::mutex> lock(path_mutex_);
            if (current_path_ != current_path_.root_path()) {
                next_path_ = current_path_.parent_path();
                request_file_list_update();
            }
        });
        refresh_btn_ = WidgetBuilder::create_button("🔄 Refresh", [this] {
            request_file_list_update();
        });
        open_btn_ = WidgetBuilder::create_button("📂 Open", [this] {
            int selected = file_menu_->get_selected();
            if (selected >= 0 && selected < static_cast<int>(files_.size()))
            {
                std::filesystem::path selected_path = files_[selected];
                if (std::filesystem::is_directory(selected_path))
                {
                    std::unique_lock<std::mutex> lock(path_mutex_);
                    next_path_ = selected_path;
                    request_file_list_update();
                }
                else
                {
                    app_.show_message("Opening file: " + selected_path.filename().string(), "File Action");
                }
            }
        });

        root_container_ = ftxui::Container::Vertical({
            ftxui::Container::Horizontal({
                up_btn_->get_component(),
                refresh_btn_->get_component(),
                open_btn_->get_component()
            }),
            Renderer([this] { return file_menu_->render(); })
        });
    }

    ~FileExplorerPage()
    {
        running_ = false;
        path_cv_.notify_one();
        if (fs_worker_thread_.joinable()) {
            fs_worker_thread_.join();
        }
    }

    bool OnEvent(ftxui::Event event) override {
               if (event == ftxui::Event::Custom) {
            update_ui_data();
            return true;
        }
        return ftxui::ComponentBase::OnEvent(event);
    }

    ftxui::Component get_component() {
        return root_container_;
    }

    ftxui::Element Render() override {
        std::string path_display = "Current Path: " + current_path_.string();
        std::string error_display = "";
        {
            std::lock_guard<std::mutex> lock(error_message_mutex_);
            if (error_occurred_) {
                error_display = "Error: " + error_message_;
            }
        }

        return ftxui::vbox({
            ftxui::text(path_display) | ftxui::bold | ftxui::color(ftxui::Color::Cyan),
            error_display.empty() ? ftxui::text("") : ftxui::text(error_display) | ftxui::color(ftxui::Color::Red),
            root_container_->Render() | ftxui::flex
        }) | ftxui::border | ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, 50);
    }

private:
    Application &app_;
    std::filesystem::path current_path_;
    std::filesystem::path next_path_;
    std::vector<std::filesystem::path> files_;
    std::vector<std::string> file_display_names_;
    std::shared_ptr<StyledMenu> file_menu_;
    std::shared_ptr<TUIKButton> up_btn_;
    std::shared_ptr<TUIKButton> refresh_btn_;
    std::shared_ptr<TUIKButton> open_btn_;

    ftxui::Component root_container_;

    std::thread fs_worker_thread_;
    std::atomic<bool> running_;
    std::atomic<bool> refresh_requested_;
    std::mutex path_mutex_;
    std::condition_variable path_cv_;
    std::atomic<bool> error_occurred_;
    std::string error_message_;
    std::mutex error_message_mutex_;

    std::vector<std::filesystem::path> new_files_;
    std::vector<std::string> new_file_display_names_;
    std::mutex data_mutex_;
    std::atomic<bool> new_data_ready_;

    void request_file_list_update() {
        refresh_requested_ = true;
        path_cv_.notify_one();
    }

    void fs_worker_loop()
    {
        while (running_)
        {
            std::unique_lock<std::mutex> lock(path_mutex_);
            path_cv_.wait(lock, [this] { return refresh_requested_.load() || !running_.load(); });

            if (!running_) {                break;
            }

            if (current_path_ != next_path_) {
                current_path_ = next_path_;
            }

            std::vector<std::filesystem::path> temp_files;
            std::vector<std::string> temp_file_display_names;

            if (current_path_ != current_path_.root_path()) {
                temp_files.push_back(current_path_.parent_path());
                temp_file_display_names.push_back("↑ ..");
            }

            try {
                for (const auto &entry : std::filesystem::directory_iterator(current_path_))
                {
                    temp_files.push_back(entry.path());
                    std::string display_name = entry.path().filename().string();
                    if (std::filesystem::is_directory(entry.status()))
                    {
                        display_name = "📁 " + display_name + "/";
                    } else {
                        display_name = "📄 " + display_name;
                    }
                    temp_file_display_names.push_back(display_name);
                }
                std::lock_guard<std::mutex> error_lock(error_message_mutex_);
                error_occurred_ = false;
                error_message_ = "";
            } catch (const std::filesystem::filesystem_error& e) {
                std::lock_guard<std::mutex> error_lock(error_message_mutex_);
                error_occurred_ = true;
                error_message_ = "Error accessing directory: " + std::string(e.what());
            }

            {
                std::lock_guard<std::mutex> data_lock(data_mutex_);
                new_files_ = std::move(temp_files);
                new_file_display_names_ = std::move(temp_file_display_names);
                new_data_ready_ = true;
            }

            app_.PostEvent(ftxui::Event::Custom);
            refresh_requested_ = false;
        }
    }

    void update_ui_data() {
        if (new_data_ready_.load()) {
            std::lock_guard<std::mutex> data_lock(data_mutex_);
            files_ = std::move(new_files_);
            file_display_names_ = std::move(new_file_display_names_);
            file_menu_->set_options(file_display_names_);
            new_data_ready_ = false;
            app_.request_redraw();
        }
    }
};

// Theme Page
class ThemePage
{
public:
    ThemePage(Application &app) : app_(app)
    {
        next_theme_button_ = WidgetBuilder::create_button("Next Theme", [this]
                                                          {
            theme_index_ = (theme_index_ + 1) % themes_.size();
            app_.set_theme(themes_[theme_index_]);
            app_.request_redraw(); });
    }

    Component get_component()
    {
        auto container = Container::Vertical({next_theme_button_->get_component()});
        return Renderer(container, [&]
                        { return vbox(std::vector<Element>{
                                     text("Current Theme: " + theme_names_[theme_index_]) | center,
                                     text("Example Text") | color(ThemeManager::instance().get_theme().primary) | center,
                                     next_theme_button_->get_component()->Render() | center,
                                 }) |
                                 border | center; });
    }

private:
    Application &app_;
    int theme_index_ = 0;
    const std::vector<Theme> themes_ = {Theme::Default, Theme::Dark, Theme::Light, Theme::Monokai};
    const std::vector<std::string> theme_names_ = {"Default", "Dark", "Light", "Monokai"};
    std::shared_ptr<TUIKButton> next_theme_button_;
};

// Quit Page
class QuitPage
{
public:
    QuitPage(Application &app) : app_(app)
    {
        quit_button_ = WidgetBuilder::create_button("Quit Application", [this]
                                                    {
            app_.confirm("Do you really want to quit?", [&](bool confirmed) {
                if (confirmed) {
                    app_.exit();
                } else {
                    app_.request_redraw();
                }
            }); });
    }

    Component get_component()
    {
        auto container = Container::Vertical({quit_button_->get_component()});
        return Renderer(container, [&]
                        { return quit_button_->get_component()->Render() | center; });
    }

private:
    Application &app_;
    std::shared_ptr<TUIKButton> quit_button_;
};

// =============================================================================
// MAIN APPLICATION
// =============================================================================
class MainApp
{
public:
    MainApp() : app_("🚀 TUIKIT Framework - Demos")
    {
        home_page_ = std::make_shared<HomePage>();
        form_page_ = std::make_shared<FormPage>(app_);
        theme_page_ = std::make_shared<ThemePage>(app_);
        quit_page_ = std::make_shared<QuitPage>(app_);
        task_manager_page_ = std::make_shared<TaskManagerPage>(app_);
        calculator_page_ = std::make_shared<CalculatorPage>(app_);
        file_explorer_page_ = std::make_shared<FileExplorerPage>(app_);
        system_monitor_page_ = std::make_shared<SystemMonitorPage>(app_);
        new_components_page_ = std::make_shared<NewComponentsPage>(app_);

        app_.add_page("🏠 Home", home_page_->get_component());
        app_.add_page("📝 Form", form_page_->get_component());
        app_.add_page("❓ About", Renderer([]
                                           { return vbox({
                                                        text("ℹ️  About") | bold | center | color(Color::Blue),
                                                        text("") | center,
                                                        text("TUIKIT Framework v0.2") | center | bold,
                                                        text("") | center,
                                                        text("Created with ❤️  in C++ and FTXUI") | center,
                                                        text("") | center,
                                                        text("Key Features:") | color(Color::Yellow),
                                                        text("✓ Modern User Interface") | color(Color::Green),
                                                        text("✓ Reusable Components") | color(Color::Green),
                                                        text("✓ Event System") | color(Color::Green),
                                                        text("✓ Customizable Themes") | color(Color::Green),
                                                        text("✓ Intuitive API") | color(Color::Green),
                                                        text("") | center,
                                                        text("Technologies Used:") | color(Color::Yellow),
                                                        text("• C++17/20") | color(Color::White),
                                                        text("• FTXUI (interface)") | color(Color::White),
                                                        text("• CMake (build)") | color(Color::White),
                                                        text("") | center,
                                                        text("🔗 GitHub: https://github.com/votre-repo/tui-framework") | center | color(Color::Cyan),
                                                    }) |
                                                    border | center; }));
        app_.add_page("🎨 Themes", theme_page_->get_component());
        app_.add_page("✅ Tasks", task_manager_page_->get_component());
        app_.add_page("🔢 Calculator", calculator_page_->get_component());
        app_.add_page("📁 Files", file_explorer_page_);
        app_.add_page("📊 System", system_monitor_page_->get_component());
        app_.add_page("✨ New Components", new_components_page_->get_component());
        app_.add_page("❌ Quit", quit_page_->get_component());
    }

    void run()
    {
        app_.set_theme(Theme::Light);
        app_.show_status_bar(true);
        app_.set_status_text("Use arrows to navigate, Enter to select");
        app_.set_active_page(6); // Set File Explorer as default page (index 6)
        app_.run();
    }

private:
    Application app_;
    std::shared_ptr<HomePage> home_page_;
    std::shared_ptr<FormPage> form_page_;
    std::shared_ptr<ThemePage> theme_page_;
    std::shared_ptr<QuitPage> quit_page_;
    std::shared_ptr<TaskManagerPage> task_manager_page_;
    std::shared_ptr<CalculatorPage> calculator_page_;
    std::shared_ptr<FileExplorerPage> file_explorer_page_;
    std::shared_ptr<SystemMonitorPage> system_monitor_page_;
    std::shared_ptr<NewComponentsPage> new_components_page_;
};

// =============================================================================
// POINT D'ENTRÉE
// =============================================================================
int main()
{
    try
    {
        MainApp app;
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur capturée: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
