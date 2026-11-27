#include "tuikit/widgets/TUIForm.h"
#include "tuikit/core/TUIStyle.h"
#include <algorithm>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace TUIKIT {

namespace {
/// Columns between the label column and the field.
constexpr int kLabelGap = 2;
/// Upper bound for the shared label column width; longer labels are clipped.
constexpr int kMaxLabelColumnWidth = 32;
} // namespace

TUIForm::TUIForm(TUIWidget* /*parent*/)
    : TUIWidget(ftxui::Container::Vertical({})) {
}

ftxui::Component TUIForm::makeRow(const std::string& label, const std::shared_ptr<TUIWidget>& field) {
    // All rows share the same label column width (TUIForm::label_column_width_),
    // so the fields line up like in a GUI form.
    auto label_renderer = ftxui::Renderer([label, width = label_column_width_] {
        auto& theme = TUIStyle::instance().currentTheme();
        return ftxui::text(label) | ftxui::color(theme.text) |
               ftxui::size(ftxui::WIDTH, ftxui::EQUAL, width);
    });
    auto gutter = ftxui::Renderer([] {
        return ftxui::text(std::string(kLabelGap, ' '));
    });
    return ftxui::Container::Horizontal({
        label_renderer,
        gutter,
        field->get_ftxui_component()
    });
}

void TUIForm::addField(const std::string& label, std::shared_ptr<TUIWidget> field) {
    const std::string focus_label = focusedLabel();
    fields_.push_back({label, field});
    // Rebuild every row so all of them share the updated label column width.
    rebuildRows(focus_label);
}

void TUIForm::removeField(const std::string& label) {
    const std::string focus_label = focusedLabel();
    for (size_t i = 0; i < fields_.size(); ++i) {
        if (fields_[i].first == label) {
            fields_.erase(fields_.begin() + static_cast<long>(i));
            rebuildRows(focus_label);
            return;
        }
    }
}

void TUIForm::clearFields() {
    fields_.clear();
    component_->DetachAllChildren();
    rows_.clear();
    label_column_width_ = 1;
    requestRedraw();
}

void TUIForm::setFields(const std::vector<FormField>& fields) {
    const std::string focus_label = focusedLabel();
    fields_.clear();
    for (const auto& field : fields) {
        fields_.push_back({field.label, field.widget});
    }
    rebuildRows(focus_label);
}

std::string TUIForm::focusedLabel() const {
    // Walk the active-child chain: form -> row -> field component.
    const ftxui::ComponentBase* node = component_.get();
    while (node) {
        ftxui::Component active = const_cast<ftxui::ComponentBase*>(node)->ActiveChild();
        if (!active) {
            break;
        }
        for (const auto& entry : fields_) {
            if (active.get() == entry.second->get_ftxui_component().get()) {
                return entry.first;
            }
        }
        node = active.get();
    }
    return "";
}

void TUIForm::rebuildRows(const std::string& focus_label) {
    label_column_width_ = 1;
    for (const auto& entry : fields_) {
        label_column_width_ = std::min(kMaxLabelColumnWidth, std::max(label_column_width_, static_cast<int>(entry.first.size())));
    }
    component_->DetachAllChildren();
    rows_.clear();
    for (const auto& entry : fields_) {
        auto row = makeRow(entry.first, entry.second);
        rows_.push_back(row);
        component_->Add(row);
    }
    restoreFocus(focus_label);
    requestRedraw();
}

void TUIForm::restoreFocus(const std::string& label) {
    if (label.empty()) {
        return;
    }
    for (size_t i = 0; i < fields_.size() && i < rows_.size(); ++i) {
        if (fields_[i].first == label) {
            component_->SetActiveChild(rows_[i].get());
            rows_[i]->SetActiveChild(fields_[i].second->get_ftxui_component().get());
            return;
        }
    }
}

} // namespace TUIKIT
