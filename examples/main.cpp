#include "tuikit.h"
#include "tuikit/core/TUIIcons.h"
#include "tuikit/widgets/Notification.h"
#include <iostream>
#include <string>
#include <fstream> // Required for file redirection

using namespace TUIKIT;

int main() {
    // Redirect std::cerr to a file
    std::ofstream cerr_log("debug_log.txt");
    std::streambuf* old_cerr_buf = std::cerr.rdbuf();
    std::cerr.rdbuf(cerr_log.rdbuf());

    TUIApp app("My TUIKIT Application");

    // Labels for displaying widget states
    auto input_label = label("Input: ");
    auto button_label = label("Button clicked: No");
    auto menu_label = label("Selected Menu Item: None");
    auto checkbox_label = label("Checkbox: Off");
    auto radiobox_label = label("Selected Radio Option: None");
    auto combobox_label = label("Selected ComboBox Option: None");
    auto slider_label = label("Slider Value: 0.0");

    // --- Main Widgets Tab Content ---
    auto main_widgets_content = vbox();

    // Input & Button Group
    auto input_button_layout = hbox();
    auto text_field_widget = textfield("Enter text here");
    text_field_widget->onChange([&](const std::string& new_text) {
        input_label->setText("Input: " + new_text);
    });
    auto button_widget = button("Show Input");
    button_widget->setIcon(ICON::Rocket);
    button_widget->onClick([&] {
        button_label->setText("Button clicked: Yes. Current input: " + text_field_widget->text());
    });
    input_button_layout->addWidget(text_field_widget);
    input_button_layout->addWidget(button_widget);

    auto input_button_group = vbox();
    input_button_group->addWidget(input_button_layout);
    input_button_group->addWidget(input_label);
    input_button_group->addWidget(button_label);

    // Notification Example
    auto show_notification_button = button("Show Notification");
    show_notification_button->onClick([&app] {
        Notification::show(app, "Hello from TUIKIT!", "Info", 3000);
    });
    input_button_group->addWidget(show_notification_button);

    main_widgets_content->addWidget(groupbox("Input & Button", input_button_group));

    // Selection Widgets Group
    auto selection_widgets_layout = vbox();

    std::vector<std::string> menu_options = {"Option 1", "Option 2", "Option 3"};
    auto menu_widget = menu(menu_options);
    std::vector<std::string> menu_icons = {ICON::NewFile, ICON::Open, ICON::Tasks};
    menu_widget->setIcons(menu_icons);
    menu_widget->onSelect([&](int selected_index) {
        menu_label->setText("Selected Menu Item: " + menu_widget->selectedText() + " (Index: " + std::to_string(selected_index) + ")");
    });
    selection_widgets_layout->addWidget(menu_widget);
    selection_widgets_layout->addWidget(menu_label);

    auto checkbox_widget = checkbox("Enable Feature");
    checkbox_widget->onChange([&](bool checked) {
        checkbox_label->setText("Checkbox: " + std::string(checked ? "On" : "Off"));
    });
    selection_widgets_layout->addWidget(checkbox_widget);
    selection_widgets_layout->addWidget(checkbox_label);

    std::vector<std::string> radio_options = {"Red", "Green", "Blue"};
    auto radiobox_widget = radiobox(radio_options);
    radiobox_widget->onSelect([&](int selected_index) {
        radiobox_label->setText("Selected Radio Option: " + radiobox_widget->selectedText() + " (Index: " + std::to_string(selected_index) + ")");
    });
    selection_widgets_layout->addWidget(radiobox_widget);
    selection_widgets_layout->addWidget(radiobox_label);

    std::vector<std::string> combobox_options = {"Apple", "Banana", "Cherry"};
    auto combobox_widget = combobox(combobox_options);
    combobox_widget->onSelect([&](int selected_index) {
        combobox_label->setText("Selected ComboBox Option: " + combobox_widget->selectedText() + " (Index: " + std::to_string(selected_index) + ")");
        app.request_redraw();
    });
    selection_widgets_layout->addWidget(combobox_widget);
    selection_widgets_layout->addWidget(combobox_label);

    main_widgets_content->addWidget(groupbox("Selection Widgets", selection_widgets_layout));

    // Slider Group
    float slider_value = 0.0f;
    auto slider_widget = slider("Volume", slider_value, 0.0f, 100.0f, 1.0f);
    slider_widget->onChange([&](float value) {
        slider_label->setText("Slider Value: " + std::to_string(static_cast<int>(value)));
        app.request_redraw();
    });
    auto slider_group = vbox();
    slider_group->addWidget(slider_widget);
    slider_group->addWidget(slider_label);
    main_widgets_content->addWidget(groupbox("Slider", slider_group));

    // --- Collapsible Tab Content ---
    auto collapsible_section_content = vbox();
    collapsible_section_content->addWidget(label("This is inside a collapsible section."));
    collapsible_section_content->addWidget(label("Expand me to see more!"));
    collapsible_section_content->addWidget(button("Toggle Me"));
    auto collapsible_widget = collapsible("Advanced Options", collapsible_section_content);
    auto collapsible_tab_content = vbox();
    collapsible_tab_content->addWidget(collapsible_widget);

    // --- Themes Tab Content ---
    auto theme_tab_content = vbox();
    auto theme_label = label("Change the global theme of the application.");
    theme_tab_content->addWidget(theme_label);
    theme_tab_content->addWidget(label("Select Theme:"));
    std::vector<std::string> theme_options = {"Dark", "Light", "Monokai", "Solarized", "Dracula", "Nord", "Blueberry", "GitHub", "Material", "MaterialDark", "MaterialLight", "MaterialBlue"};
    auto theme_combobox = combobox(theme_options, 0);
    theme_combobox->onSelect([&](int idx) {
        TUIStyle::instance().setGlobalTheme(static_cast<Theme>(idx));
        app.request_redraw();
    });
    theme_tab_content->addWidget(theme_combobox);

    // --- Form & Status Tab Content ---
    auto form_status_tab_content = vbox();
    auto form_widget = form();
    form_widget->addField("Name:", textfield("Your Name"));
    form_widget->addField("Email:", textfield("Your Email"));
    form_status_tab_content->addWidget(groupbox("User Information", form_widget));

    auto status_bar = statusbar("Application Ready.");
    auto update_status_button = button("Update Status");
    update_status_button->onClick([&] {
        static int status_count = 0;
        status_bar->setMessage("Status updated: " + std::to_string(++status_count));
    });
    auto status_group = vbox();
    status_group->addWidget(status_bar);
    status_group->addWidget(update_status_button);
    form_status_tab_content->addWidget(groupbox("Application Status", status_group));

    // --- Toolbar Tab Content ---
    auto toolbar_tab_content = vbox();
    auto toolbar_widget = toolbar();
    toolbar_widget->addButton("New", [] { std::cerr << "New button clicked!" << std::endl; }, ICON::NewFile);
    toolbar_widget->addButton("Save", [] { std::cerr << "Save button clicked!" << std::endl; }, ICON::Save);
    toolbar_widget->addButton("Load", [] { std::cerr << "Load button clicked!" << std::endl; }, ICON::Open);
    toolbar_widget->addButton("Quit", [&app] { app.exit(); }, ICON::Quit);
    toolbar_tab_content->addWidget(groupbox("Application Toolbar", toolbar_widget));

    // --- Advanced Widgets Tab Content ---
    auto advanced_widgets_content = vbox();

    // TUITreeView Example
    TreeNode root_tree_node = {"Root", {
        {"Child 1", {}},
        {"Child 2", {
            {"Grandchild 2.1", {}},
            {"Grandchild 2.2", {}}
        }},
        {"Child 3", {}}
    }};
    auto tree_view = treeview(root_tree_node);
    auto tree_view_label = label("Selected Tree Item: None");
    tree_view->onSelect([&](const std::string& selected_node_text) {
        tree_view_label->setText("Selected Tree Item: " + selected_node_text);
    });
    auto tree_view_group = vbox();
    tree_view_group->addWidget(tree_view);
    tree_view_group->addWidget(tree_view_label);
    advanced_widgets_content->addWidget(groupbox("Tree View", tree_view_group));

    // --- Tab Widget ---
    auto tab_widget = tabwidget();
    tab_widget->addTab("Main Widgets", main_widgets_content, ICON::Home);
    tab_widget->addTab("Collapsible", collapsible_tab_content, ICON::Collapsible);
    tab_widget->addTab("Themes", theme_tab_content, ICON::Themes);
    tab_widget->addTab("Form & Status", form_status_tab_content, ICON::Tasks);
    tab_widget->addTab("Toolbar", toolbar_tab_content, ICON::NewFile);
    tab_widget->addTab("Advanced Widgets", advanced_widgets_content, ICON::Tasks);

    // --- Main Layout ---
    auto main_layout = vbox();
    main_layout->addWidget(tab_widget);

    app.setMainWidget(main_layout);

    app.setOnExit([&] {
        std::cerr << "Application is exiting. Goodbye!" << std::endl;
        std::cerr.rdbuf(old_cerr_buf);
    });

    int result = app.exec();

    std::cerr.rdbuf(old_cerr_buf);

    return result;
}