#pragma once

#include "tuikit/core/TUIApp.h"
#include "tuikit/core/TUIWidget.h"
#include "tuikit/layouts/TUIHBoxLayout.h"
#include "tuikit/layouts/TUIVBoxLayout.h"
#include "tuikit/widgets/TUIButton.h"
#include "tuikit/widgets/TUICheckBox.h"
#include "tuikit/widgets/TUICollapsible.h"
#include "tuikit/widgets/TUIComboBox.h"
#include "tuikit/widgets/TUIForm.h"
#include "tuikit/widgets/TUIGroupBox.h"
#include "tuikit/widgets/TUILabel.h"
#include "tuikit/widgets/TUIMenu.h"
#include "tuikit/widgets/TUIProgressBar.h"
#include "tuikit/widgets/TUIRadioBox.h"
#include "tuikit/widgets/TUIResizableSplit.h"
#include "tuikit/widgets/TUIScrollableContainer.h"
#include "tuikit/widgets/TUISlider.h"
#include "tuikit/widgets/TUIStatusBar.h"
#include "tuikit/widgets/TUITabWidget.h"
#include "tuikit/widgets/TUITextArea.h"
#include "tuikit/widgets/TUITextField.h"
#include "tuikit/widgets/TUIToolbar.h"
#include "tuikit/widgets/TUITreeView.h"

#include "json.hpp" // Include nlohmann/json
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace TUIKIT {

class TUIKLoader {
public:
    TUIKLoader();
    std::shared_ptr<TUIWidget> load(const std::string& filepath);
    std::shared_ptr<TUIWidget> getWidget(const std::string& name);

private:
    std::map<std::string, std::shared_ptr<TUIWidget>> named_widgets_;

    std::shared_ptr<TUIWidget> createWidgetFromJson(const nlohmann::json& widget_json);
    void applyProperties(std::shared_ptr<TUIWidget> widget, const nlohmann::json& properties_json);
};

} // namespace TUIKIT
