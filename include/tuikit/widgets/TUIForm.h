#pragma once
#include "tuikit/core/TUIWidget.h"
#include <memory>
#include <string>
#include <vector>
#include <ftxui/component/component.hpp>

namespace TUIKIT {

/// One labelled field of a TUIForm.
struct FormField {
    std::string label;
    std::shared_ptr<TUIWidget> widget;
};

/// Labelled field container with support for dynamic add/remove/rebuild,
/// e.g. forms generated from JSON schemas with conditional branches where
/// fields appear and disappear depending on other values.
class TUIForm : public TUIWidget {
public:
    TUIForm(TUIWidget* parent = nullptr);

    void addField(const std::string& label, std::shared_ptr<TUIWidget> field);
    /// Remove the first field with the given label (no-op if absent).
    void removeField(const std::string& label);
    /// Remove all fields.
    void clearFields();
    /// Replace the whole field list. Focus is preserved on the field with the
    /// same label as the focused one before the rebuild, when it still exists.
    void setFields(const std::vector<FormField>& fields);

    /// Overrides the shared label column width (the default: the longest
    /// label, capped). A host that knows its pane width calls this before
    /// setFields/addField so a narrow pane keeps a visible field — the
    /// labels truncate to fit.
    void setLabelColumnWidth(int width) { label_column_width_override_ = width; }

private:
    std::string focusedLabel() const;
    void rebuildRows(const std::string& focus_label);
    void restoreFocus(const std::string& label);
    ftxui::Component makeRow(const std::string& label, const std::shared_ptr<TUIWidget>& field);

    std::vector<std::pair<std::string, std::shared_ptr<TUIWidget>>> fields_;
    std::vector<ftxui::Component> rows_;
    /// Shared label column width: max label length, capped. Kept in sync by
    /// addField()/rebuildRows()/clearFields() so every row aligns.
    int label_column_width_ = 1;
    /// When set (>= 1), replaces the computed width (the host's pane-aware
    /// override — a narrow pane keeps a visible field).
    int label_column_width_override_ = -1;
};
} // namespace TUIKIT
