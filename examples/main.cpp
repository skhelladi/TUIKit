#include "tuikit.h"
#include "tuikit/core/TUIIcons.h"
#include <iostream>
#include <string>
#include <memory> // Required for std::shared_ptr

int main() {
    TUIKIT::TUIApp app("My TUIKIT Application");

    // Labels for displaying widget states
    auto input_label = std::make_shared<TUIKIT::TUILabel>("Input: ");
    auto button_label = std::make_shared<TUIKIT::TUILabel>("Button clicked: No");
    auto menu_label = std::make_shared<TUIKIT::TUILabel>("Selected Menu Item: None");
    auto checkbox_label = std::make_shared<TUIKIT::TUILabel>("Checkbox: Off");
    auto radiobox_label = std::make_shared<TUIKIT::TUILabel>("Selected Radio Option: None");
    auto combobox_label = std::make_shared<TUIKIT::TUILabel>("Selected ComboBox Option: None");
    auto slider_label = std::make_shared<TUIKIT::TUILabel>("Slider Value: 0.0");

    // --- Main Widgets Tab Content ---
    auto main_widgets_content = std::make_shared<TUIKIT::TUIVBoxLayout>();

    // Input & Button Group
    auto input_button_layout = std::make_shared<TUIKIT::TUIHBoxLayout>();
    auto text_field = std::make_shared<TUIKIT::TUITextField>("Enter text here");
    text_field->onChange([&](const std::string& new_text) {
        input_label->setText("Input: " + new_text);
    });
    auto button = std::make_shared<TUIKIT::TUIButton>("Show Input");
    button->setIcon(TUIKIT::ICON::Rocket);
    button->onClick([&] {
        button_label->setText("Button clicked: Yes. Current input: " + text_field->text());
    });
    input_button_layout->addWidget(text_field);
    input_button_layout->addWidget(button);

    auto input_button_group = std::make_shared<TUIKIT::TUIVBoxLayout>();
    input_button_group->addWidget(input_button_layout);
    input_button_group->addWidget(input_label);
    input_button_group->addWidget(button_label);
    auto input_button_group_box = std::make_shared<TUIKIT::TUIGroupBox>("Input & Button", input_button_group);
    main_widgets_content->addWidget(input_button_group_box);

    // Selection Widgets Group
    auto selection_widgets_layout = std::make_shared<TUIKIT::TUIVBoxLayout>();

    std::vector<std::string> menu_options = {"Option 1", "Option 2", "Option 3"};
    auto menu = std::make_shared<TUIKIT::TUIMenu>(menu_options);
    std::vector<std::string> menu_icons = {TUIKIT::ICON::NewFile, TUIKIT::ICON::Open, TUIKIT::ICON::Tasks};
    menu->setIcons(menu_icons);
    menu->onSelect([&](int selected_index) {
        menu_label->setText("Selected Menu Item: " + menu->selectedText() + " (Index: " + std::to_string(selected_index) + ")");
    });
    selection_widgets_layout->addWidget(menu);
    selection_widgets_layout->addWidget(menu_label);

    auto checkbox = std::make_shared<TUIKIT::TUICheckBox>("Enable Feature");
    checkbox->onChange([&](bool checked) {
        checkbox_label->setText("Checkbox: " + std::string(checked ? "On" : "Off"));
    });
    selection_widgets_layout->addWidget(checkbox);
    selection_widgets_layout->addWidget(checkbox_label);

    std::vector<std::string> radio_options = {"Red", "Green", "Blue"};
    auto radiobox = std::make_shared<TUIKIT::TUIRadioBox>(radio_options);
    radiobox->onSelect([&](int selected_index) {
        radiobox_label->setText("Selected Radio Option: " + radiobox->selectedText() + " (Index: " + std::to_string(selected_index) + ")");
    });
    selection_widgets_layout->addWidget(radiobox);
    selection_widgets_layout->addWidget(radiobox_label);

    std::vector<std::string> combobox_options = {"Apple", "Banana", "Cherry"};
    auto combobox = std::make_shared<TUIKIT::TUIComboBox>(combobox_options);
    combobox->onSelect([&](int selected_index) {
        combobox_label->setText("Selected ComboBox Option: " + combobox->selectedText() + " (Index: " + std::to_string(selected_index) + ")");
        app.request_redraw();
    });
    selection_widgets_layout->addWidget(combobox);
    selection_widgets_layout->addWidget(combobox_label);

    auto selection_widgets_group_box = std::make_shared<TUIKIT::TUIGroupBox>("Selection Widgets", selection_widgets_layout);
    main_widgets_content->addWidget(selection_widgets_group_box);

    // Slider Group
    float slider_value = 0.0f;
    auto slider = std::make_shared<TUIKIT::TUISlider>("Volume", slider_value, 0.0f, 100.0f, 1.0f);
    slider->onChange([&](float value) {
        slider_label->setText("Slider Value: " + std::to_string(static_cast<int>(value)));
        app.request_redraw();
    });
    auto slider_group = std::make_shared<TUIKIT::TUIVBoxLayout>();
    slider_group->addWidget(slider);
    slider_group->addWidget(slider_label);
    auto slider_group_box = std::make_shared<TUIKIT::TUIGroupBox>("Slider", slider_group);
    main_widgets_content->addWidget(slider_group_box);

    // --- Collapsible Tab Content ---
    auto collapsible_section_content = std::make_shared<TUIKIT::TUIVBoxLayout>();
    collapsible_section_content->addWidget(std::make_shared<TUIKIT::TUILabel>("This is inside a collapsible section."));
    collapsible_section_content->addWidget(std::make_shared<TUIKIT::TUILabel>("Expand me to see more!"));
    collapsible_section_content->addWidget(std::make_shared<TUIKIT::TUIButton>("Toggle Me"));
    auto collapsible = std::make_shared<TUIKIT::TUICollapsible>("Advanced Options", collapsible_section_content);
    auto collapsible_tab_content = std::make_shared<TUIKIT::TUIVBoxLayout>();
    collapsible_tab_content->addWidget(collapsible);

    // --- Themes Tab Content ---
    auto theme_tab_content = std::make_shared<TUIKIT::TUIVBoxLayout>();
    auto theme_label = std::make_shared<TUIKIT::TUILabel>("Select Theme:");
    std::vector<std::string> theme_options = {"Dark", "Light", "Monokai", "Solarized", "Dracula", "Nord", "Blueberry", "GitHub", "Material", "MaterialDark", "MaterialLight", "MaterialBlue"};
    int selected_theme = 0; // Default: Dark
    auto theme_combobox = std::make_shared<TUIKIT::TUIComboBox>(theme_options, selected_theme);
    theme_combobox->onSelect([&](int idx) {
        if (theme_options[idx] == "Dark")
            TUIKIT::TUIStyle::instance().setGlobalTheme(TUIKIT::Theme::Dark);
        else if (theme_options[idx] == "Light")
            TUIKIT::TUIStyle::instance().setGlobalTheme(TUIKIT::Theme::Light);
        else if (theme_options[idx] == "Monokai")
            TUIKIT::TUIStyle::instance().setGlobalTheme(TUIKIT::Theme::Monokai);
        else if (theme_options[idx] == "Solarized")
            TUIKIT::TUIStyle::instance().setGlobalTheme(TUIKIT::Theme::Solarized);
        else if (theme_options[idx] == "Dracula")
            TUIKIT::TUIStyle::instance().setGlobalTheme(TUIKIT::Theme::Dracula);
        else if (theme_options[idx] == "Nord")
            TUIKIT::TUIStyle::instance().setGlobalTheme(TUIKIT::Theme::Nord);
        else if (theme_options[idx] == "Blueberry")
            TUIKIT::TUIStyle::instance().setGlobalTheme(TUIKIT::Theme::Blueberry);
        else if (theme_options[idx] == "GitHub")
            TUIKIT::TUIStyle::instance().setGlobalTheme(TUIKIT::Theme::GitHub);
        else if (theme_options[idx] == "Material")
            TUIKIT::TUIStyle::instance().setGlobalTheme(TUIKIT::Theme::Material);
        else if (theme_options[idx] == "MaterialDark")
            TUIKIT::TUIStyle::instance().setGlobalTheme(TUIKIT::Theme::MaterialDark);
        else if (theme_options[idx] == "MaterialLight")
            TUIKIT::TUIStyle::instance().setGlobalTheme(TUIKIT::Theme::MaterialLight);
        else if (theme_options[idx] == "MaterialBlue")
            TUIKIT::TUIStyle::instance().setGlobalTheme(TUIKIT::Theme::MaterialBlue);
        app.request_redraw();
    });
    theme_tab_content->addWidget(theme_label);
    theme_tab_content->addWidget(theme_combobox);

    // --- Form & Status Tab Content ---
    auto form_status_tab_content = std::make_shared<TUIKIT::TUIVBoxLayout>();

    // Form Group
    auto form = std::make_shared<TUIKIT::TUIForm>();
    auto form_name_field = std::make_shared<TUIKIT::TUITextField>("Your Name");
    auto form_email_field = std::make_shared<TUIKIT::TUITextField>("Your Email");
    form->addField("Name:", form_name_field);
    form->addField("Email:", form_email_field);
    auto form_group_box = std::make_shared<TUIKIT::TUIGroupBox>("User Information", form);
    form_status_tab_content->addWidget(form_group_box);

    // Status Bar Group
    auto status_bar = std::make_shared<TUIKIT::TUIStatusBar>("Application Ready.");
    auto update_status_button = std::make_shared<TUIKIT::TUIButton>("Update Status");
    update_status_button->onClick([&] {
        static int status_count = 0;
        status_bar->setMessage("Status updated: " + std::to_string(++status_count));
    });
    auto status_group = std::make_shared<TUIKIT::TUIVBoxLayout>();
    status_group->addWidget(status_bar);
    status_group->addWidget(update_status_button);
    auto status_group_box = std::make_shared<TUIKIT::TUIGroupBox>("Application Status", status_group);
    form_status_tab_content->addWidget(status_group_box);

    // --- Toolbar Tab Content ---
    auto toolbar_tab_content = std::make_shared<TUIKIT::TUIVBoxLayout>();
    auto toolbar = std::make_shared<TUIKIT::TUIToolbar>();
    toolbar->addButton("New", [] { std::cerr << "New button clicked!" << std::endl; }, TUIKIT::ICON::NewFile);
    toolbar->addButton("Save", [] { std::cerr << "Save button clicked!" << std::endl; }, TUIKIT::ICON::Save);
    toolbar->addButton("Load", [] { std::cerr << "Load button clicked!" << std::endl; }, TUIKIT::ICON::Open);
    toolbar->addButton("Quit", [&app] { app.exit(); }, TUIKIT::ICON::Quit);
    auto toolbar_group_box = std::make_shared<TUIKIT::TUIGroupBox>("Application Toolbar", toolbar);
    toolbar_tab_content->addWidget(toolbar_group_box);

    // --- Tab Widget ---
    auto tab_widget = std::make_shared<TUIKIT::TUITabWidget>();
    tab_widget->addTab("Main Widgets", main_widgets_content, TUIKIT::ICON::Home);
    tab_widget->addTab("Collapsible", collapsible_tab_content, TUIKIT::ICON::Collapsible);
    tab_widget->addTab("Themes", theme_tab_content, TUIKIT::ICON::Themes);
    tab_widget->addTab("Form & Status", form_status_tab_content, TUIKIT::ICON::Tasks);
    tab_widget->addTab("Toolbar", toolbar_tab_content, TUIKIT::ICON::NewFile);

    // --- Main Layout ---
    auto main_layout = std::make_shared<TUIKIT::TUIVBoxLayout>();
    main_layout->addWidget(tab_widget);

    app.setMainWidget(main_layout);

    app.setOnExit([] {
        std::cerr << "Application is exiting. Goodbye!" << std::endl;
    });

    return app.exec();
}