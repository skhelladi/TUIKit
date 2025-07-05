#pragma once

// Main TUIKit include

#include "tuikit/core/TUIApp.h"
#include "tuikit/core/TUIWidget.h"
#include "tuikit/core/TUIStyle.h"
#include "tuikit/layouts/TUILayout.h"
#include "tuikit/layouts/TUIHBoxLayout.h"
#include "tuikit/layouts/TUIVBoxLayout.h"
#include "tuikit/widgets/TUIButton.h"
#include "tuikit/widgets/TUITextField.h"
#include "tuikit/widgets/TUILabel.h"
#include "tuikit/widgets/TUIMenu.h"
#include "tuikit/widgets/TUICheckBox.h"
#include "tuikit/widgets/TUIRadioBox.h"
#include "tuikit/widgets/TUIComboBox.h"
#include "tuikit/widgets/TUISlider.h"
#include "tuikit/widgets/TUIResizableSplit.h"
#include "tuikit/widgets/TUICollapsible.h"
#include "tuikit/widgets/TUITabWidget.h"
#include "tuikit/widgets/TUIGroupBox.h"
#include "tuikit/widgets/TUIForm.h"
#include "tuikit/widgets/TUIStatusBar.h"
#include "tuikit/widgets/TUIToolbar.h"
#include "tuikit/widgets/TUITreeView.h"
#include "tuikit/widgets/TUITextArea.h"
#include "tuikit/widgets/TUIProgressBar.h"
#include "tuikit/widgets/TUIScrollableContainer.h"
#include "tuikit/core/TUIConnect.h"

#include "tuikit/core/TUIIcons.h"
#include "tuikit/widgets/Notification.h"



#include <memory>
#include <string>
#include <vector>

namespace TUIKIT {

    // Widget Aliases
    using Widget = std::shared_ptr<TUIWidget>;
    using Button = std::shared_ptr<TUIButton>;
    using CheckBox = std::shared_ptr<TUICheckBox>;
    using Collapsible = std::shared_ptr<TUICollapsible>;
    using ComboBox = std::shared_ptr<TUIComboBox>;
    using Form = std::shared_ptr<TUIForm>;
    using GroupBox = std::shared_ptr<TUIGroupBox>;
    using Label = std::shared_ptr<TUILabel>;
    using Menu = std::shared_ptr<TUIMenu>;
    using ProgressBar = std::shared_ptr<TUIProgressBar>;
    using RadioBox = std::shared_ptr<TUIRadioBox>;
    using ResizableSplit = std::shared_ptr<TUIResizableSplit>;
    using ScrollableContainer = std::shared_ptr<TUIScrollableContainer>;
    using Slider = std::shared_ptr<TUISlider>;
    using StatusBar = std::shared_ptr<TUIStatusBar>;
    using TabWidget = std::shared_ptr<TUITabWidget>;
    using TextArea = std::shared_ptr<TUITextArea>;
    using TextField = std::shared_ptr<TUITextField>;
    using Toolbar = std::shared_ptr<TUIToolbar>;
    using TreeView = std::shared_ptr<TUITreeView>;

    // Layout Aliases
    using Layout = std::shared_ptr<TUILayout>;
    using HBoxLayout = std::shared_ptr<TUIHBoxLayout>;
    using VBoxLayout = std::shared_ptr<TUIVBoxLayout>;

    // Factory Functions
    inline Button button(const std::string& text) {
        return std::make_shared<TUIButton>(text);
    }

    inline CheckBox checkbox(const std::string& text, bool initial_checked = false) {
        return std::make_shared<TUICheckBox>(text, initial_checked);
    }

    inline Collapsible collapsible(const std::string& title, Widget content) {
        return std::make_shared<TUICollapsible>(title, content);
    }

    inline ComboBox combobox(const std::vector<std::string>& options, int initial_selected = 0) {
        return std::make_shared<TUIComboBox>(options, initial_selected);
    }

    inline Form form() {
        return std::make_shared<TUIForm>();
    }

    inline GroupBox groupbox(const std::string& title, Widget content, bool show_border = true) {
        return std::make_shared<TUIGroupBox>(title, content, show_border);
    }

    inline Label label(const std::string& text = "") {
        return std::make_shared<TUILabel>(text);
    }

    inline Menu menu(const std::vector<std::string>& options) {
        return std::make_shared<TUIMenu>(options);
    }

    inline ProgressBar progressbar(float initial_value = 0.0f, const std::string& label = "") {
        return std::make_shared<TUIProgressBar>(initial_value, label);
    }

    inline RadioBox radiobox(const std::vector<std::string>& options, int initial_selected = 0) {
        return std::make_shared<TUIRadioBox>(options, initial_selected);
    }

    inline ResizableSplit resizable_split(Widget first, Widget second, TUIResizableSplit::Orientation orientation = TUIResizableSplit::Horizontal) {
        return std::make_shared<TUIResizableSplit>(first, second, orientation);
    }

    inline ScrollableContainer scrollable_container(Widget content, int height) {
        return std::make_shared<TUIScrollableContainer>(content, height);
    }

    inline Slider slider(const std::string& label, float initial_value, float min_value, float max_value, float increment) {
        return std::make_shared<TUISlider>(label, initial_value, min_value, max_value, increment);
    }

    inline StatusBar statusbar(const std::string& message = "") {
        return std::make_shared<TUIStatusBar>(message);
    }

    inline TabWidget tabwidget() {
        return std::make_shared<TUITabWidget>();
    }

    inline TextArea textarea(const std::string& placeholder = "", const std::string& label = "", int height = 5) {
        return std::make_shared<TUITextArea>(placeholder, label, height);
    }

    inline TextField textfield(const std::string& placeholder = "") {
        return std::make_shared<TUITextField>(placeholder);
    }

    inline Toolbar toolbar() {
        return std::make_shared<TUIToolbar>();
    }

    inline TreeView treeview(TreeNode root_node) {
        return std::make_shared<TUITreeView>(root_node);
    }

    // Layout Factories
    inline HBoxLayout hbox() {
        return std::make_shared<TUIHBoxLayout>();
    }

    inline VBoxLayout vbox() {
        return std::make_shared<TUIVBoxLayout>();
    }
}
