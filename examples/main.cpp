#include "tuikit.h"
#include "tuikit/core/TUIIcons.h"
#include <iostream>
#include <string>
#include <memory> // Required for std::shared_ptr

int main() {
    TUIKIT::TUIApp app("My TUIKIT Application");

    std::cerr << "[main] - Creating main_layout." << std::endl;
    auto main_layout = std::make_shared<TUIKIT::TUIVBoxLayout>();

    // Create a horizontal layout for the input field and button
    auto input_button_layout = std::make_shared<TUIKIT::TUIHBoxLayout>();

    auto input_label = std::make_shared<TUIKIT::TUILabel>("Input: ");
    auto button_label = std::make_shared<TUIKIT::TUILabel>("Button clicked: No");
    auto menu_label = std::make_shared<TUIKIT::TUILabel>("Selected Menu Item: None");
    auto checkbox_label = std::make_shared<TUIKIT::TUILabel>("Checkbox: Off");
    auto radiobox_label = std::make_shared<TUIKIT::TUILabel>("Selected Radio Option: None");
    auto combobox_label = std::make_shared<TUIKIT::TUILabel>("Selected ComboBox Option: None");
    auto slider_label = std::make_shared<TUIKIT::TUILabel>("Slider Value: 0.0");

    auto text_field = std::make_shared<TUIKIT::TUITextField>("Enter text here");
    text_field->onChange([&](const std::string& new_text) {
        input_label->setText("Input: " + new_text);
    });

    auto button = std::make_shared<TUIKIT::TUIButton>("Show Input");
    button->setIcon(TUIKIT::ICON::Rocket); // Exemple d'icône à gauche du texte
    button->onClick([&] {
        button_label->setText("Button clicked: Yes. Current input: " + text_field->text());
    });

    input_button_layout->addWidget(text_field);
    input_button_layout->addWidget(button);

    main_layout->addWidget(input_label);
    main_layout->addWidget(button_label);
    main_layout->addWidget(input_button_layout);
    main_layout->addWidget(menu_label);
    main_layout->addWidget(checkbox_label);
    main_layout->addWidget(radiobox_label);
    main_layout->addWidget(combobox_label);
    main_layout->addWidget(slider_label);

    std::vector<std::string> menu_options = {"Option 1", "Option 2", "Option 3"};
    auto menu = std::make_shared<TUIKIT::TUIMenu>(menu_options);
    std::vector<std::string> menu_icons = {TUIKIT::ICON::NewFile, TUIKIT::ICON::Open, TUIKIT::ICON::Tasks};
    menu->setIcons(menu_icons);
    menu->onSelect([&](int selected_index) {
        menu_label->setText("Selected Menu Item: " + menu->selectedText() + " (Index: " + std::to_string(selected_index) + ")");
    });

    main_layout->addWidget(menu);

    auto checkbox = std::make_shared<TUIKIT::TUICheckBox>("Enable Feature");
    checkbox->onChange([&](bool checked) {
        checkbox_label->setText("Checkbox: " + std::string(checked ? "On" : "Off"));
    });

    main_layout->addWidget(checkbox);

    std::vector<std::string> radio_options = {"Red", "Green", "Blue"};
    auto radiobox = std::make_shared<TUIKIT::TUIRadioBox>(radio_options);
    radiobox->onSelect([&](int selected_index) {
        radiobox_label->setText("Selected Radio Option: " + radiobox->selectedText() + " (Index: " + std::to_string(selected_index) + ")");
    });

    main_layout->addWidget(radiobox);

    std::vector<std::string> combobox_options = {"Apple", "Banana", "Cherry"};
    auto combobox = std::make_shared<TUIKIT::TUIComboBox>(combobox_options);
    combobox->onSelect([&](int selected_index) {
        combobox_label->setText("Selected ComboBox Option: " + combobox->selectedText() + " (Index: " + std::to_string(selected_index) + ")");
        app.request_redraw();
    });

    main_layout->addWidget(combobox);

    float slider_value = 0.0f;
    auto slider = std::make_shared<TUIKIT::TUISlider>("Volume", slider_value, 0.0f, 100.0f, 1.0f);
    slider->onChange([&](float value) {
        slider_label->setText("Slider Value: " + std::to_string(static_cast<int>(value)));
        app.request_redraw();
    });

    main_layout->addWidget(slider);

    // Left panel for the split
    auto left_panel_content = std::make_shared<TUIKIT::TUIVBoxLayout>();
    left_panel_content->addWidget(menu);
    left_panel_content->addWidget(input_label);
    left_panel_content->addWidget(input_button_layout);
    left_panel_content->addWidget(button_label);
    left_panel_content->addWidget(menu_label);

    auto left_group_box = std::make_shared<TUIKIT::TUIGroupBox>("Input & Menu", left_panel_content);

    // Right panel for the split
    auto right_panel_content = std::make_shared<TUIKIT::TUIVBoxLayout>();
    right_panel_content->addWidget(checkbox);
    right_panel_content->addWidget(checkbox_label);
    right_panel_content->addWidget(radiobox);
    right_panel_content->addWidget(radiobox_label);
    right_panel_content->addWidget(combobox);
    right_panel_content->addWidget(combobox_label);
    right_panel_content->addWidget(slider);
    right_panel_content->addWidget(slider_label);

    auto right_group_box = std::make_shared<TUIKIT::TUIGroupBox>("Controls", right_panel_content);

    // Create a collapsible section for some widgets
    auto collapsible_section_content = std::make_shared<TUIKIT::TUIVBoxLayout>();
    collapsible_section_content->addWidget(std::make_shared<TUIKIT::TUILabel>("This is inside a collapsible section."));
    collapsible_section_content->addWidget(std::make_shared<TUIKIT::TUILabel>("Expand me to see more!"));
    collapsible_section_content->addWidget(std::make_shared<TUIKIT::TUIButton>("Toggle Me"));

    auto collapsible = std::make_shared<TUIKIT::TUICollapsible>("Advanced Options", collapsible_section_content);

    // Create a resizable split between the two panels with min sizes
    auto resizable_split = std::make_shared<TUIKIT::TUIResizableSplit>(left_group_box, right_group_box, TUIKIT::TUIResizableSplit::Horizontal);
    resizable_split->setMinimumSizes(50, 20); // e.g. 50 columns min for left side, 20 for right side

    // Create Tab 1 content
    auto tab1_content = std::make_shared<TUIKIT::TUIVBoxLayout>();
    tab1_content->addWidget(resizable_split);

    // Create Tab 2 content
    auto tab2_content = std::make_shared<TUIKIT::TUIVBoxLayout>();
    tab2_content->addWidget(collapsible);
    tab2_content->addWidget(std::make_shared<TUIKIT::TUILabel>("This is content for Tab 2."));

    // Create Tab 3: Theme selection
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

    // Create the Tab Widget
    auto tab_widget = std::make_shared<TUIKIT::TUITabWidget>();
    tab_widget->addTab("Main Widgets", tab1_content, TUIKIT::ICON::Home);
    tab_widget->addTab("Collapsible", tab2_content, TUIKIT::ICON::Collapsible);
    tab_widget->addTab("Themes", theme_tab_content, TUIKIT::ICON::Themes);

    main_layout->addWidget(tab_widget);

    // Create a form
    auto form = std::make_shared<TUIKIT::TUIForm>();
    auto form_name_field = std::make_shared<TUIKIT::TUITextField>("Your Name");
    auto form_email_field = std::make_shared<TUIKIT::TUITextField>("Your Email");
    form->addField("Name:", form_name_field);
    form->addField("Email:", form_email_field);

    auto form_group_box = std::make_shared<TUIKIT::TUIGroupBox>("User Information", form);

    main_layout->addWidget(form_group_box);

    // Create a status bar
    auto status_bar = std::make_shared<TUIKIT::TUIStatusBar>("Application Ready.");
    auto update_status_button = std::make_shared<TUIKIT::TUIButton>("Update Status");
    update_status_button->onClick([&] {
        static int status_count = 0;
        status_bar->setMessage("Status updated: " + std::to_string(++status_count));
    });

    // Create a toolbar
    auto toolbar = std::make_shared<TUIKIT::TUIToolbar>();
    toolbar->addButton("New", [] { std::cerr << "New button clicked!" << std::endl; }, TUIKIT::ICON::NewFile);
    toolbar->addButton("Save", [] { std::cerr << "Save button clicked!" << std::endl; }, TUIKIT::ICON::Save);
    toolbar->addButton("Load", [] { std::cerr << "Load button clicked!" << std::endl; }, TUIKIT::ICON::Open);
    toolbar->addButton("Quit", [&app] { app.exit(); }, TUIKIT::ICON::Quit);

    main_layout->addWidget(toolbar);
    main_layout->addWidget(update_status_button);
    main_layout->addWidget(status_bar);

    app.setMainWidget(main_layout);

    app.setOnExit([] {
        std::cerr << "Application is exiting. Goodbye!" << std::endl;
    });

    return app.exec();
}
