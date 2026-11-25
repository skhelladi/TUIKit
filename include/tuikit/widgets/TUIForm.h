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

private:
    std::string focusedLabel() const;
    void rebuildRows(const std::string& focus_label);
    void restoreFocus(const std::string& label);
    ftxui::Component makeRow(const std::string& label, const std::shared_ptr<TUIWidget>& field);

    std::vector<std::pair<std::string, std::shared_ptr<TUIWidget>>> fields_;
    std::vector<ftxui::Component> rows_;
};
} // namespace TUIKIT
