
#include "tuikit/widgets/Notification.h"
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include "tuikit/core/TUIApp.h"

namespace TUIKIT {

void Notification::show(TUIApp& app, const std::string& message, const std::string& title, int duration_ms) {
    auto notification_content = ftxui::vbox({
        ftxui::text(title) | ftxui::bold,
        ftxui::separator(),
        ftxui::paragraph(message)
    });

    auto notification_element = notification_content | ftxui::border | ftxui::size(ftxui::WIDTH, ftxui::LESS_THAN, 50);

    auto modal_container = ftxui::Renderer([notification_element] {
        std::vector<ftxui::Element> hbox_elements;
        hbox_elements.push_back(ftxui::text("") | ftxui::flex);
        hbox_elements.push_back(notification_element);

        std::vector<ftxui::Element> vbox_elements;
        vbox_elements.push_back(ftxui::text("") | ftxui::flex);
        vbox_elements.push_back(ftxui::hbox(hbox_elements));

        return ftxui::vbox(vbox_elements);
    });

    app.show_modal(modal_container);

    std::thread([&app, duration_ms]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms));
        app.close_modal();
    }).detach();
}

} // namespace TUIKIT
