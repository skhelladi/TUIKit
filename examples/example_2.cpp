#include "tuikit.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace TUIKIT;

int main() {
    std::ofstream cerr_log("debug_log_example2.txt");
    std::streambuf* old_cerr_buf = std::cerr.rdbuf();
    std::cerr.rdbuf(cerr_log.rdbuf());

    TUIApp app("TUIKIT UI Loader Example");

    // Load UI from .tuik file
    auto main_ui_widget = app.loadUI("../examples/simple_ui.tuik");

    if (!main_ui_widget) {
        std::cerr << "Failed to load UI from simple_ui.tuik" << std::endl;
        std::cout << "Failed to load UI from simple_ui.tuik" << std::endl;
        return 1;
    }

    app.setMainWidget(main_ui_widget);

    // Get named widgets
    auto name_input = std::dynamic_pointer_cast<TUITextField>(app.getWidget("name_input"));
    auto submit_button = std::dynamic_pointer_cast<TUIButton>(app.getWidget("submit_button"));
    auto output_label = std::dynamic_pointer_cast<TUILabel>(app.getWidget("output_label"));

    if (name_input && submit_button && output_label) {
        connect(submit_button, [&] {
            output_label->setText("Hello, " + name_input->text() + "!");
        });
    } else {
        std::cerr << "Error: Could not find all required widgets by name." << std::endl;
    }

    app.setOnExit([&] {
        std::cerr << "Application is exiting. Goodbye from example_2!" << std::endl;
        std::cerr.rdbuf(old_cerr_buf);
    });

    int result = app.exec();

    std::cerr.rdbuf(old_cerr_buf);

    return result;

    std::cerr.rdbuf(old_cerr_buf);

    return result;
}
