#ifndef TUIKIT_NOTIFICATION_H
#define TUIKIT_NOTIFICATION_H

#include "tuikit/core/TUIApp.h"
#include <string>
#include <thread>
#include <chrono>

namespace TUIKIT {

class Notification {
public:
    static void show(TUIApp& app, const std::string& message, const std::string& title = "Notification", int duration_ms = 3000);
};

} // namespace TUIKIT

#endif // TUIKIT_NOTIFICATION_H
