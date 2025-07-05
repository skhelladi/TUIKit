#include "tuikit/core/TUIKLoader.h"
#include "tuikit.h" // For factory functions
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

namespace TUIKIT
{

    TUIKLoader::TUIKLoader() {}

    std::shared_ptr<TUIWidget> TUIKLoader::load(const std::string &filepath)
    {
        std::ifstream file(filepath);
        if (!file.is_open())
        {
            std::cerr << "Error: Could not open .tuik file: " << filepath << std::endl;
            std::cout << "Error: Could not open .tuik file: " << filepath << std::endl;
            return nullptr;
        }
        else
            std::cerr << "[DEBUG] TUIKLoader::load: Successfully opened .tuik file: " << filepath << std::endl;

        nlohmann::json tuik_json;
        try
        {
            file >> tuik_json;
        }
        catch (const nlohmann::json::parse_error &e)
        {
            std::cerr << "Error parsing .tuik file: " << e.what() << std::endl;
            return nullptr;
        }

        return createWidgetFromJson(tuik_json);
    }

    std::shared_ptr<TUIWidget> TUIKLoader::getWidget(const std::string &name)
    {
        if (named_widgets_.count(name))
        {
            return named_widgets_[name];
        }
        return nullptr;
    }

    std::shared_ptr<TUIWidget> TUIKLoader::createWidgetFromJson(const nlohmann::json &widget_json)
    {
        std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Entering function." << std::endl
                  << std::flush;
        if (!widget_json.contains("class"))
        {
            std::cerr << "[ERROR] TUIKLoader::createWidgetFromJson: Widget JSON missing 'class' field." << std::endl
                      << std::flush;
            return nullptr;
        }

        std::string class_name = widget_json["class"];
        std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Class name: " << class_name << std::endl
                  << std::flush;
        std::shared_ptr<TUIWidget> widget = nullptr;

        // Instantiate widget based on class_name
        if (class_name == "TUILabel")
        {
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Creating TUILabel." << std::endl
                      << std::flush;
            widget = label();
        }
        else if (class_name == "TUIButton")
        {
            std::string button_text = widget_json.contains("properties") && widget_json["properties"].contains("text") ? widget_json["properties"]["text"] : "";
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Creating TUIButton with text: " << button_text << std::endl
                      << std::flush;
            widget = button(button_text);
        }
        else if (class_name == "TUITextField")
        {
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Creating TUITextField." << std::endl
                      << std::flush;
            widget = textfield();
        }
        else if (class_name == "TUIVBoxLayout")
        {
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Creating TUIVBoxLayout." << std::endl
                      << std::flush;
            widget = vbox();
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: TUIVBoxLayout created." << std::endl
                      << std::flush;
        }
        else if (class_name == "TUIHBoxLayout")
        {
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Creating TUIHBoxLayout." << std::endl
                      << std::flush;
            widget = hbox();
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: TUIHBoxLayout created." << std::endl
                      << std::flush;
        }
        else if (class_name == "TUICheckBox")
        {
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Creating TUICheckBox." << std::endl
                      << std::flush;
            widget = checkbox("");
        }
        else if (class_name == "TUIRadioBox")
        {
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Creating TUIRadioBox." << std::endl
                      << std::flush;
            widget = radiobox({});
        }
        else if (class_name == "TUIComboBox")
        {
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Creating TUIComboBox." << std::endl
                      << std::flush;
            widget = combobox({});
        }
        else if (class_name == "TUISlider")
        {
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Creating TUISlider." << std::endl
                      << std::flush;
            widget = slider("", 0.0f, 0.0f, 100.0f, 1.0f); // Default values, will be overridden
        }
        else if (class_name == "TUIGroupBox")
        {
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Creating TUIGroupBox." << std::endl
                      << std::flush;
            widget = groupbox("", nullptr); // Title and content will be set by properties/children
        }
        else if (class_name == "TUIForm")
        {
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Creating TUIForm." << std::endl
                      << std::flush;
            widget = form();
        }
        else if (class_name == "TUITabWidget")
        {
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Creating TUITabWidget." << std::endl
                      << std::flush;
            widget = tabwidget();
        }
        else if (class_name == "TUIStatusBar")
        {
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Creating TUIStatusBar." << std::endl
                      << std::flush;
            widget = statusbar();
        }
        else if (class_name == "TUIToolbar")
        {
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Creating TUIToolbar." << std::endl
                      << std::flush;
            widget = toolbar();
        }
        else if (class_name == "TUITreeView")
        {
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Creating TUITreeView." << std::endl
                      << std::flush;
            // TUITreeView requires a TreeNode, which is complex to define in JSON directly.
            // For now, we'll skip direct instantiation or require a specific structure.
            std::cerr << "Warning: TUITreeView instantiation from JSON is not fully supported yet." << std::endl
                      << std::flush;
            return nullptr;
        }
        else if (class_name == "TUIResizableSplit")
        {
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Creating TUIResizableSplit." << std::endl
                      << std::flush;
            // TUIResizableSplit requires two child widgets, handled below.
            widget = resizable_split(nullptr, nullptr); // Will be set later
        }
        else if (class_name == "TUIScrollableContainer")
        {
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Creating TUIScrollableContainer." << std::endl
                      << std::flush;
            widget = scrollable_container(nullptr, 0); // Will be set later
        }
        else if (class_name == "TUITextArea")
        {
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Creating TUITextArea." << std::endl
                      << std::flush;
            widget = textarea();
        }
        else if (class_name == "TUIProgressBar")
        {
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Creating TUIProgressBar." << std::endl
                      << std::flush;
            widget = progressbar();
        }
        else if (class_name == "TUICollapsible")
        {
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Creating TUICollapsible." << std::endl
                      << std::flush;
            widget = collapsible("", nullptr);
        }
        else
        {
            std::cerr << "[ERROR] TUIKLoader::createWidgetFromJson: Unknown widget class: " << class_name << std::endl
                      << std::flush;
            return nullptr;
        }

        if (!widget)
        {
            std::cerr << "[ERROR] TUIKLoader::createWidgetFromJson: Widget creation failed for class: " << class_name << std::endl
                      << std::flush;
            return nullptr;
        }

        // Store named widgets
        if (widget_json.contains("name"))
        {
            std::string name = widget_json["name"];
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Storing named widget: " << name << std::endl
                      << std::flush;
            named_widgets_[name] = widget;
        }

        // Apply properties
        if (widget_json.contains("properties"))
        {
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Applying properties." << std::endl
                      << std::flush;
            applyProperties(widget, widget_json["properties"]);
        }

        // Handle children for layouts
        if (widget_json.contains("children") && widget_json["children"].is_array())
        {
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Handling children for layout." << std::endl
                      << std::flush;
            if (auto vlayout = std::dynamic_pointer_cast<TUIVBoxLayout>(widget))
            {
                for (const auto &child_json : widget_json["children"])
                {
                    std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Creating child widget." << std::endl
                              << std::flush;
                    if (auto child_widget = createWidgetFromJson(child_json))
                    {
                        vlayout->addWidget(child_widget);
                    }
                }
            }
            else if (auto hlayout = std::dynamic_pointer_cast<TUIHBoxLayout>(widget))
            {
                for (const auto &child_json : widget_json["children"])
                {
                    std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Creating child widget." << std::endl
                              << std::flush;
                    if (auto child_widget = createWidgetFromJson(child_json))
                    {
                        hlayout->addWidget(child_widget);
                    }
                }
            }
            else
            {
                std::cerr << "[WARNING] TUIKLoader::createWidgetFromJson: 'children' field found for non-layout widget: " << class_name << std::endl
                          << std::flush;
            }
        }

        // Handle content for single-child containers (e.g., TUIGroupBox, TUICollapsible, TUIScrollableContainer)
        if (widget_json.contains("content") && widget_json["content"].is_object())
        {
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Handling content for single-child container." << std::endl
                      << std::flush;
            if (auto group_box = std::dynamic_pointer_cast<TUIGroupBox>(widget))
            {
                std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Setting content for GroupBox." << std::endl
                          << std::flush;
                if (auto content_widget = createWidgetFromJson(widget_json["content"]))
                {
                    group_box->setContentWidget(content_widget);
                }
            }
            else if (auto collapsible_widget = std::dynamic_pointer_cast<TUICollapsible>(widget))
            {
                std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Setting content for Collapsible." << std::endl
                          << std::flush;
                if (auto content_widget = createWidgetFromJson(widget_json["content"]))
                {
                    // Collapsible constructor takes content directly
                    // Re-instantiate to pass content correctly
                    std::string title = widget_json.contains("properties") && widget_json["properties"].contains("title") ? widget_json["properties"]["title"] : "";
                    widget = collapsible(title, content_widget);
                    if (widget_json.contains("name"))
                    {
                        named_widgets_[widget_json["name"]] = widget;
                    }
                }
            }
            else if (auto scrollable_container_widget = std::dynamic_pointer_cast<TUIScrollableContainer>(widget))
            {
                std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Setting content for ScrollableContainer." << std::endl
                          << std::flush;
                if (auto content_widget = createWidgetFromJson(widget_json["content"]))
                {
                    int height = widget_json.contains("properties") && widget_json["properties"].contains("height") ? widget_json["properties"]["height"].get<int>() : 0;
                    widget = scrollable_container(content_widget, height);
                    if (widget_json.contains("name"))
                    {
                        named_widgets_[widget_json["name"]] = widget;
                    }
                }
            }
            else
            {
                std::cerr << "[WARNING] TUIKLoader::createWidgetFromJson: 'content' field found for widget that doesn't explicitly handle it: " << class_name << std::endl
                          << std::flush;
            }
        }

        // Special handling for TUIResizableSplit (requires two children)
        if (class_name == "TUIResizableSplit" && widget_json.contains("children") && widget_json["children"].is_array() && widget_json["children"].size() == 2)
        {
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Handling children for ResizableSplit." << std::endl
                      << std::flush;
            auto first_child = createWidgetFromJson(widget_json["children"][0]);
            auto second_child = createWidgetFromJson(widget_json["children"][1]);
            if (first_child && second_child)
            {
                TUIResizableSplit::Orientation orientation_enum = TUIResizableSplit::Horizontal;
                if (widget_json.contains("properties") && widget_json["properties"].contains("orientation"))
                {
                    std::string orientation_str = widget_json["properties"]["orientation"];
                    if (orientation_str == "Vertical")
                    {
                        orientation_enum = TUIResizableSplit::Vertical;
                    }
                }
                widget = resizable_split(first_child, second_child, orientation_enum);
                if (widget_json.contains("name"))
                {
                    named_widgets_[widget_json["name"]] = widget;
                }
            }
        }

        // Special handling for TUIForm (requires fields with labels)
        if (class_name == "TUIForm" && widget_json.contains("fields") && widget_json["fields"].is_array())
        {
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Handling fields for TUIForm." << std::endl
                      << std::flush;
            if (auto form_widget = std::dynamic_pointer_cast<TUIForm>(widget))
            {
                for (const auto &field_json : widget_json["fields"])
                {
                    if (field_json.contains("label") && field_json.contains("widget"))
                    {
                        std::string field_label = field_json["label"];
                        std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Creating form field widget." << std::endl
                                  << std::flush;
                        if (auto field_content_widget = createWidgetFromJson(field_json["widget"]))
                        {
                            form_widget->addField(field_label, field_content_widget);
                        }
                    }
                }
            }
        }

        // Special handling for TUITabWidget (requires tabs with titles and content)
        if (class_name == "TUITabWidget" && widget_json.contains("tabs") && widget_json["tabs"].is_array())
        {
            std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Handling tabs for TUITabWidget." << std::endl
                      << std::flush;
            if (auto tab_widget = std::dynamic_pointer_cast<TUITabWidget>(widget))
            {
                for (const auto &tab_json : widget_json["tabs"])
                {
                    if (tab_json.contains("title") && tab_json.contains("content"))
                    {
                        std::string tab_title = tab_json["title"];
                        std::string tab_icon = tab_json.contains("icon") ? tab_json["icon"] : "";
                        std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Creating tab content widget." << std::endl
                                  << std::flush;
                        if (auto tab_content_widget = createWidgetFromJson(tab_json["content"]))
                        {
                            if (!tab_icon.empty())
                            {
                                tab_widget->addTab(tab_title, tab_content_widget, tab_icon);
                            }
                            else
                            {
                                tab_widget->addTab(tab_title, tab_content_widget);
                            }
                        }
                    }
                }
            }
        }
        std::cerr << "[DEBUG] TUIKLoader::createWidgetFromJson: Exiting function." << std::endl
                  << std::flush;
        return widget;
    }

    void TUIKLoader::applyProperties(std::shared_ptr<TUIWidget> widget, const nlohmann::json &properties_json)
    {
        if (!widget)
            return;

        // Common properties
        if (properties_json.contains("text"))
        {
            if (auto label_widget = std::dynamic_pointer_cast<TUILabel>(widget))
            {
                label_widget->setText(properties_json["text"]);
            }
            else if (auto button_widget = std::dynamic_pointer_cast<TUIButton>(widget))
            {
                button_widget->setText(properties_json["text"]);
            }
            else if (auto checkbox_widget = std::dynamic_pointer_cast<TUICheckBox>(widget))
            {
                // Checkbox text is set via constructor, no direct setter.
            }
            else if (auto statusbar_widget = std::dynamic_pointer_cast<TUIStatusBar>(widget))
            {
                statusbar_widget->setMessage(properties_json["text"]);
            }
            else if (auto textarea_widget = std::dynamic_pointer_cast<TUITextArea>(widget))
            {
                textarea_widget->setText(properties_json["text"]);
            }
            else if (auto textfield_widget = std::dynamic_pointer_cast<TUITextField>(widget))
            {
                // TextField text is set via constructor, no direct setter.
            }
        }

        if (properties_json.contains("placeholder"))
        {
            if (auto textfield_widget = std::dynamic_pointer_cast<TUITextField>(widget))
            {
                // TextField placeholder is set via constructor, no direct setter.
            }
            else if (auto textarea_widget = std::dynamic_pointer_cast<TUITextArea>(widget))
            {
                // TextArea placeholder is set via constructor, no direct setter.
            }
        }

        if (properties_json.contains("icon"))
        {
            if (auto button_widget = std::dynamic_pointer_cast<TUIButton>(widget))
            {
                std::string icon_name = properties_json["icon"];
                // Map string icon name to TUIKIT::ICON:: constants
                if (icon_name == "Rocket")
                    button_widget->setIcon(ICON::Rocket);
                else if (icon_name == "NewFile")
                    button_widget->setIcon(ICON::NewFile);
                else if (icon_name == "Open")
                    button_widget->setIcon(ICON::Open);
                else if (icon_name == "Save")
                    button_widget->setIcon(ICON::Save);
                else if (icon_name == "Tasks")
                    button_widget->setIcon(ICON::Tasks);
                else if (icon_name == "Quit")
                    button_widget->setIcon(ICON::Quit);
                else if (icon_name == "Info")
                    button_widget->setIcon(ICON::Info);
                else if (icon_name == "Warning")
                    button_widget->setIcon(ICON::Warning);
                else if (icon_name == "Success")
                    button_widget->setIcon(ICON::Success);
                else if (icon_name == "Home")
                    button_widget->setIcon(ICON::Home);
                else if (icon_name == "Collapsible")
                    button_widget->setIcon(ICON::Collapsible);
                else if (icon_name == "Themes")
                    button_widget->setIcon(ICON::Themes);
                else
                    button_widget->setIcon(icon_name); // Fallback to raw string if not a known icon
            }
        }

        if (properties_json.contains("title"))
        {
            if (auto group_box_widget = std::dynamic_pointer_cast<TUIGroupBox>(widget))
            {
                group_box_widget->setTitle(properties_json["title"]);
            }
            else if (auto collapsible_widget = std::dynamic_pointer_cast<TUICollapsible>(widget))
            {
                // Collapsible title is set via constructor, no direct setter.
            }
        }

        if (properties_json.contains("show_border"))
        {
            if (auto group_box_widget = std::dynamic_pointer_cast<TUIGroupBox>(widget))
            {
                group_box_widget->setShowBorder(properties_json["show_border"]);
            }
        }

        if (properties_json.contains("initial_checked"))
        {
            if (auto checkbox_widget = std::dynamic_pointer_cast<TUICheckBox>(widget))
            {
                // Checkbox initial_checked is set via constructor, no direct setter.
            }
        }

        if (properties_json.contains("options") && properties_json["options"].is_array())
        {
            std::vector<std::string> options;
            for (const auto &opt : properties_json["options"])
            {
                options.push_back(opt.get<std::string>());
            }
            if (auto menu_widget = std::dynamic_pointer_cast<TUIMenu>(widget))
            {
                menu_widget->setOptions(options);
            }
            else if (auto combobox_widget = std::dynamic_pointer_cast<TUIComboBox>(widget))
            {
                combobox_widget->setOptions(options);
            }
            else if (auto radiobox_widget = std::dynamic_pointer_cast<TUIRadioBox>(widget))
            {
                radiobox_widget->setOptions(options);
            }
        }

        if (properties_json.contains("initial_selected"))
        {
            int initial_selected = properties_json["initial_selected"];
            if (auto combobox_widget = std::dynamic_pointer_cast<TUIComboBox>(widget))
            {
                combobox_widget->setSelectedIndex(initial_selected);
            }
            else if (auto radiobox_widget = std::dynamic_pointer_cast<TUIRadioBox>(widget))
            {
                radiobox_widget->setSelectedIndex(initial_selected);
            }
            else if (auto menu_widget = std::dynamic_pointer_cast<TUIMenu>(widget))
            {
                menu_widget->setSelectedIndex(initial_selected);
            }
        }

        if (properties_json.contains("value"))
        {
            if (auto slider_widget = std::dynamic_pointer_cast<TUISlider>(widget))
            {
                slider_widget->setValue(properties_json["value"]);
            }
            else if (auto progressbar_widget = std::dynamic_pointer_cast<TUIProgressBar>(widget))
            {
                progressbar_widget->setValue(properties_json["value"]);
            }
        }

        if (properties_json.contains("min_value"))
        {
            // Sliders min/max/increment are set via constructor, no direct setters.
        }
        if (properties_json.contains("max_value"))
        {
            // Sliders min/max/increment are set via constructor, no direct setters.
        }
        if (properties_json.contains("increment"))
        {
            // Sliders min/max/increment are set via constructor, no direct setters.
        }

        if (properties_json.contains("height"))
        {
            if (auto textarea_widget = std::dynamic_pointer_cast<TUITextArea>(widget))
            {
                // TextArea height is set via constructor, no direct setter.
            }
        }

        if (properties_json.contains("orientation"))
        {
            // ResizableSplit orientation is set via constructor, no direct setter.
        }

        // Add more property handling as needed for other widgets
    }

} // namespace TUIKIT
