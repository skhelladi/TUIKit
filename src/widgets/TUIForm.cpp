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
constexpr int kMaxLabelColumnWidth = 24;
} // namespace

TUIForm::TUIForm(TUIWidget* /*parent*/)
    : TUIWidget(ftxui::Container::Vertical({})) {
}

ftxui::Component TUIForm::makeRow(const std::string& label, const std::shared_ptr<TUIWidget>& field) {
    using namespace ftxui;
    // Short label: the horizontal layout — the label column (shared by every
    // row, so the fields align), the gutter, then the field. The label is
    // vertically centered in its column: the decorated fields are three
    // lines tall (frame) and a top-aligned label reads as the caption of the
    // frame's top border instead of the field's name — the value then sits
    // one line below an unlabeled line.
    auto label_renderer = Renderer([label, width = label_column_width_] {
        auto& theme = TUIStyle::instance().currentTheme();
        return text(label) | color(theme.text) | vcenter
             | size(WIDTH, EQUAL, width);
    });
    auto gutter = Renderer([] {
        return text(std::string(kLabelGap, ' ')) | vcenter;
    });
    return Container::Horizontal({label_renderer, gutter, field->get_ftxui_component()});
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
    if (label_column_width_override_ >= 1)
        label_column_width_ = std::min(label_column_width_, label_column_width_override_);
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
