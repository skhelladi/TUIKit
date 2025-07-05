#include "tuikit.h"
#include <iostream>
#include <string>
#include <fstream>
#include "example_2_tuik.h"
#include <nlohmann/json.hpp>

// Ensure the following extern declarations match those in your generated simple_ui_tuik.h

using namespace TUIKIT;

int main()
{
    std::ofstream cerr_log("debug_log_example2.txt");
    std::streambuf *old_cerr_buf = std::cerr.rdbuf();
    std::cerr.rdbuf(cerr_log.rdbuf());

    TUIApp app("TUIKIT UI Loader Example");

    // Load UI from binary resource
    std::string tuik_str(reinterpret_cast<const char *>(example_2_tuik), sizeof(example_2_tuik));
    nlohmann::json tuik_json = nlohmann::json::parse(tuik_str);
    TUIKLoader loader;
    auto main_ui_widget = loader.createWidgetFromJson(tuik_json);

    if (!main_ui_widget)
    {
        std::cerr << "Failed to load UI from embedded JSON" << std::endl;
        return 1;
    }

    app.setMainWidget(main_ui_widget);

    // Get named widgets from loader
    auto name_input = std::dynamic_pointer_cast<TUITextField>(loader.getWidget("name_input"));
    auto submit_button = std::dynamic_pointer_cast<TUIButton>(loader.getWidget("submit_button"));
    auto output_label = std::dynamic_pointer_cast<TUILabel>(loader.getWidget("output_label"));

    if (name_input && submit_button && output_label)
    {
        connect(submit_button, [&]
                { output_label->setText("Hello, " + name_input->text() + "!"); });
    }
    else
    {
        std::cerr << "Error: Could not find all required widgets by name." << std::endl;
    }

    app.setOnExit([&]
                  {
        std::cerr << "Application is exiting. Goodbye from example_2!" << std::endl;
        std::cerr.rdbuf(old_cerr_buf); });

    int result = app.exec();

    std::cerr.rdbuf(old_cerr_buf);

    return result;

    std::cerr.rdbuf(old_cerr_buf);

    return result;
}
