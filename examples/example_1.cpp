#include "tuikit.h"
#include <iostream>
#include <string>
#include <fstream> // Required for file redirection

using namespace TUIKIT;

// All comments and text are now in English.

class AppSlots {
public:
    AppSlots(TUIApp& app, Label& button_label, Label& checkbox_label) 
        : app_(app), button_label_(button_label), checkbox_label_(checkbox_label) {}

    void handleButtonClick() {
        button_label_->setText("Button clicked: Yes (Connected Slot)");
    }

    void handleCheckboxChange(bool checked) {
        checkbox_label_->setText("Checkbox: " + std::string(checked ? "On (Connected Slot)" : "Off (Connected Slot)"));
    }

    void showNotification() {
        Notification::show(app_, "Hello from a connected slot!", "Info", 3000);
    }

private:
    TUIApp& app_;
    Label& button_label_;
    Label& checkbox_label_;
};

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

    AppSlots slots(app, button_label, checkbox_label);

    // --- Main Widgets Tab Content ---
    auto main_widgets_content = vbox();

    // Input & Button Group
    auto input_button_layout = hbox();
    auto text_field_widget = textfield("Enter text here");
    connect(text_field_widget, [&](const std::string& new_text) {
        input_label->setText("Input: " + new_text);
    });

    // auto read_only_text_field = textfield("This is read-only");
    // read_only_text_field->setReadOnly(true);

    auto text_area_widget = textarea("Enter multiline text here", "", 2);
    // auto read_only_text_area = textarea("This is a read-only textarea");
    // read_only_text_area->setReadOnly(true);

    auto button_widget = button("Show Input");
    button_widget->setIcon(ICON::Rocket);
    connect(button_widget, &slots, &AppSlots::handleButtonClick);

    input_button_layout->addWidget(text_field_widget);
    // input_button_layout->addWidget(read_only_text_field);
    input_button_layout->addWidget(text_area_widget);
    // input_button_layout->addWidget(read_only_text_area);
    input_button_layout->addWidget(button_widget);

    auto input_button_group = vbox();
    input_button_group->addWidget(input_button_layout);
    input_button_group->addWidget(input_label);
    input_button_group->addWidget(button_label);

    // Notification Example
    auto show_notification_button = button("Show Notification");
    connect(show_notification_button, &slots, &AppSlots::showNotification);
    input_button_group->addWidget(show_notification_button);

    main_widgets_content->addWidget(groupbox("Input & Button", input_button_group));

    // Selection Widgets Group
    auto selection_widgets_layout = vbox();

    std::vector<std::string> menu_options = {"Option 1", "Option 2", "Option 3"};
    auto menu_widget = menu(menu_options);
    std::vector<std::string> menu_icons = {ICON::NewFile, ICON::Open, ICON::Tasks};
    menu_widget->setIcons(menu_icons);
    connect(menu_widget, [&](int selected_index) {
        menu_label->setText("Selected Menu Item: " + menu_widget->selectedText() + " (Index: " + std::to_string(selected_index) + ")");
    });
    selection_widgets_layout->addWidget(menu_widget);
    selection_widgets_layout->addWidget(menu_label);

    auto checkbox_widget = checkbox("Enable Feature");
    connect(checkbox_widget, &slots, &AppSlots::handleCheckboxChange);
    selection_widgets_layout->addWidget(checkbox_widget);
    selection_widgets_layout->addWidget(checkbox_label);

    std::vector<std::string> radio_options = {"Red", "Green", "Blue"};
    auto radiobox_widget = radiobox(radio_options);
    connect(radiobox_widget, [&](int selected_index) {
        radiobox_label->setText("Selected Radio Option: " + radiobox_widget->selectedText() + " (Index: " + std::to_string(selected_index) + ")");
    });
    selection_widgets_layout->addWidget(radiobox_widget);
    selection_widgets_layout->addWidget(radiobox_label);

    std::vector<std::string> combobox_options = {"Apple", "Banana", "Cherry"};
    auto combobox_widget = combobox(combobox_options);
    connect(combobox_widget, [&](int selected_index) {
        combobox_label->setText("Selected ComboBox Option: " + combobox_widget->selectedText() + " (Index: " + std::to_string(selected_index) + ")");
        app.request_redraw();
    });
    selection_widgets_layout->addWidget(combobox_widget);
    selection_widgets_layout->addWidget(combobox_label);

    main_widgets_content->addWidget(groupbox("Selection Widgets", selection_widgets_layout));

    // Slider Group
    float slider_value = 0.0f;
    auto slider_widget = slider("Volume", slider_value, 0.0f, 100.0f, 1.0f);
    connect(slider_widget, [&](float value) {
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
    theme_tab_content->addWidget(label("Change the global theme of the application."));
    theme_tab_content->addWidget(label("Select Theme:"));
    std::vector<std::string> theme_options = {"Dark", "Light", "Monokai", "Solarized", "Dracula", "Nord", "Blueberry", "GitHub", "Material", "MaterialDark", "MaterialLight", "MaterialBlue"};
    auto theme_combobox = combobox(theme_options, 0);
    connect(theme_combobox, [&](int idx) {
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
    connect(update_status_button, [&] {
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
    // Left side: SpinBox, ListView, TreeView, Split, Scrollable, TUITableView, spinbox, doublespinbox
    // Vertical layout: left/right with stretch
    auto left_advanced_group = vbox();
    // Vertical layout: top/bottom with stretch
    auto right_advanced_group = vbox();
    // Horizontal layout: left/right with stretch
    auto advanced_widgets_content = hbox();

    // SpinBox and DoubleSpinBox Example
    auto spin_box_group = vbox();
    auto spin_box_label = label("SpinBox Value: 50");
    auto spin_box = spinbox("Spin", 50, 0, 100, 1);
    spin_box->setValue(50);
    connect(spin_box, [&](int value) {
        spin_box_label->setText("SpinBox Value: " + std::to_string(value));
    });
    auto double_spin_box_label = label("DoubleSpinBox Value: 5.0");
    auto double_spin_box = doublespinbox("Double", 5.0, 0.0, 10.0, 0.5);
    connect(double_spin_box, [&](double value) {
        double_spin_box_label->setText("DoubleSpinBox Value: " + std::to_string(value));
    });
    spin_box_group->addWidget(spin_box);
    spin_box_group->addWidget(spin_box_label);
    spin_box_group->addWidget(double_spin_box);
    spin_box_group->addWidget(double_spin_box_label);
    right_advanced_group->addWidget(groupbox("Spin Boxes", spin_box_group));

    // TUIListView Example
    auto list_view_group = vbox();
    std::vector<std::string> list_items = {"Item 1", "Item 2", "Item 3", "Item 4", "Item 5"};
    auto list_view = listview(list_items);
    auto list_view_label = label("Selected List Item: None");
    connect(list_view, [&](int selected_index) {
        if (selected_index >= 0 && (size_t)selected_index < list_items.size()) {
            list_view_label->setText("Selected List Item: " + list_items[selected_index]);
        }
    });
    list_view_group->addWidget(list_view);
    list_view_group->addWidget(list_view_label);
    left_advanced_group->addWidget(groupbox("List View", list_view_group));

    // TUITreeView Example
    TreeNode root_tree_node = {"Root", {
        {"Child 1", {}, false, nullptr},
        {"Child 2", {
            {"Grandchild 2.1", {}, false, nullptr},
            {"Grandchild 2.2", {}, false, nullptr}
        }, false, nullptr},
        {"Child 3", {}, false, nullptr}
    }, false, nullptr};
    auto tree_view = treeview(root_tree_node);
    auto tree_view_label = label("Selected Tree Item: None");
    tree_view->onSelect([&](const std::string& selected_node_text) {
        tree_view_label->setText("Selected Tree Item: " + selected_node_text);
    });
    auto tree_view_group = vbox();
    tree_view_group->addWidget(tree_view);
    tree_view_group->addWidget(tree_view_label);
    left_advanced_group->addWidget(groupbox("Tree View", tree_view_group));

    // TUIResizableSplit Example
    auto left_panel = label("Left Panel");
    auto right_panel = label("Right Panel");
    auto resizable_split_widget = resizable_split(left_panel, right_panel, TUIResizableSplit::Horizontal);
    advanced_widgets_content->addWidget(groupbox("Resizable Split", resizable_split_widget));

    // TUIScrollableContainer Example
    std::vector<std::string> scroll_items;
    for (int i = 0; i < 20; ++i) {
        scroll_items.push_back("Scrollable Item " + std::to_string(i));
    }
    auto scroll_menu = menu(scroll_items);
    auto scroll_selected_label = label("Selected: None");
    connect(scroll_menu, [&](int idx) {
        if (idx >= 0 && idx < (int)scroll_items.size())
            scroll_selected_label->setText("Selected: " + scroll_items[idx]);
    });
    auto scrollable_container_widget = scrollable_container(scroll_menu, 10);
    auto scrollable_group = vbox();
    scrollable_group->addWidget(scrollable_container_widget);
    scrollable_group->addWidget(scroll_selected_label);
    left_advanced_group->addWidget(groupbox("Scrollable Container", scrollable_group));

    // Right side: TableView
    auto table_view_group = vbox();
    std::vector<std::string> table_headers = {"Name", "Age   ", "City   "};
    std::vector<std::vector<std::string>> table_data = {
        {"John Doe", "30", "New York"},
        {"Jane Smith", "25", "London"},
        {"Sam Green", "35", "Paris"}
    };
    auto table_view = tableview(table_headers, table_data);
    auto table_view_label = label("Selected Table Cell: None");
    connect(table_view, [&](int row, int col) {
        if (row >= 0 && col >= 0) {
            table_view_label->setText("Selected Table Cell: (Row: " + std::to_string(row) + ", Col: " + std::to_string(col) + ")");
        }
    });
    table_view_group->addWidget(table_view);
    table_view_group->addWidget(table_view_label);
    right_advanced_group->addWidget(groupbox("Table View", table_view_group));

    advanced_widgets_content->addWidget(left_advanced_group);
    advanced_widgets_content->addWidget(right_advanced_group);

    // --- Vertical Tab Widget Example ---
    auto vertical_tab_content_1 = vbox();
    vertical_tab_content_1->addWidget(label("Content for Vertical Tab 1"));
    auto vertical_tab_content_2 = vbox();
    vertical_tab_content_2->addWidget(label("Content for Vertical Tab 2"));

    auto vertical_tab_widget = tabwidget(TabOrientation::Vertical);
    vertical_tab_widget->addTab("Vert Tab 1", vertical_tab_content_1);
    vertical_tab_widget->addTab("Vert Tab 2", vertical_tab_content_2);

    // --- Tab Widget ---
    auto tab_widget = tabwidget();
    tab_widget->addTab("Main Widgets", main_widgets_content, ICON::Home);
    tab_widget->addTab("Collapsible", collapsible_tab_content, ICON::Collapsible);
    tab_widget->addTab("Themes", theme_tab_content, ICON::Themes);
    tab_widget->addTab("Form & Status", form_status_tab_content, ICON::Tasks);
    tab_widget->addTab("Toolbar", toolbar_tab_content, ICON::NewFile);
    tab_widget->addTab("Advanced Widgets", advanced_widgets_content, ICON::Tasks);
    tab_widget->addTab("Vertical Tabs", vertical_tab_widget, ICON::Tasks);

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
