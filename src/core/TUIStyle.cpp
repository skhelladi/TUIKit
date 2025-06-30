#include "tuikit/core/TUIStyle.h"

namespace TUIKIT {

TUIStyle& TUIStyle::instance() {
    static TUIStyle instance;
    return instance;
}

TUIStyle::TUIStyle() {
    initializeThemes();
    // Set default theme
    current_theme_ = themes_[Theme::Dark];
}

void TUIStyle::setGlobalTheme(Theme theme) {
    if (themes_.count(theme)) {
        current_theme_ = themes_[theme];
    }
}

const AppTheme& TUIStyle::currentTheme() const {
    return current_theme_;
}

void TUIStyle::initializeThemes() {
    // Dark Theme (Default)
    themes_[Theme::Dark] = {
        .primary = ftxui::Color::Blue,
        .secondary = ftxui::Color::Cyan,
        .success = ftxui::Color::Green,
        .warning = ftxui::Color::Yellow,
        .error = ftxui::Color::Red,
        .background = ftxui::Color::Black,
        .text = ftxui::Color::White,
        .border = ftxui::Color::GrayDark
    };

    // Light Theme
    themes_[Theme::Light] = {
        .primary = ftxui::Color::Blue,
        .secondary = ftxui::Color::Cyan,
        .success = ftxui::Color::Green,
        .warning = ftxui::Color::Yellow,
        .error = ftxui::Color::Red,
        .background = ftxui::Color::White,
        .text = ftxui::Color::Black,
        .border = ftxui::Color::GrayLight
    };
    
    // Monokai Theme
    themes_[Theme::Monokai] = {
        .primary = ftxui::Color::Magenta,
        .secondary = ftxui::Color::Cyan,
        .success = ftxui::Color::GreenLight,
        .warning = ftxui::Color::Yellow,
        .error = ftxui::Color::RedLight,
        .background = ftxui::Color::Black,
        .text = ftxui::Color::White,
        .border = ftxui::Color::GrayDark
    };

    // Solarized Theme
    themes_[Theme::Solarized] = {
        .primary = ftxui::Color::Blue,
        .secondary = ftxui::Color::Cyan,
        .success = ftxui::Color::Green,
        .warning = ftxui::Color::Yellow,
        .error = ftxui::Color::Red,
        .background = ftxui::Color::RGB(0, 43, 54),
        .text = ftxui::Color::RGB(131, 148, 150),
        .border = ftxui::Color::RGB(88, 110, 117)
    };

    // Dracula Theme
    themes_[Theme::Dracula] = {
        .primary = ftxui::Color::RGB(189, 147, 249),
        .secondary = ftxui::Color::RGB(139, 233, 253),
        .success = ftxui::Color::RGB(80, 250, 123),
        .warning = ftxui::Color::RGB(241, 250, 140),
        .error = ftxui::Color::RGB(255, 85, 85),
        .background = ftxui::Color::RGB(40, 42, 54),
        .text = ftxui::Color::RGB(248, 248, 242),
        .border = ftxui::Color::RGB(98, 114, 164)
    };

    // Nord Theme
    themes_[Theme::Nord] = {
        .primary = ftxui::Color::RGB(136, 192, 208),
        .secondary = ftxui::Color::RGB(129, 161, 193),
        .success = ftxui::Color::RGB(163, 190, 140),
        .warning = ftxui::Color::RGB(235, 203, 139),
        .error = ftxui::Color::RGB(191, 97, 106),
        .background = ftxui::Color::RGB(46, 52, 64),
        .text = ftxui::Color::RGB(216, 222, 233),
        .border = ftxui::Color::RGB(76, 86, 106)
    };

    // Blueberry Theme
    themes_[Theme::Blueberry] = {
        .primary = ftxui::Color::RGB(130, 170, 255),
        .secondary = ftxui::Color::RGB(137, 221, 255),
        .success = ftxui::Color::RGB(199, 236, 127),
        .warning = ftxui::Color::RGB(255, 204, 102),
        .error = ftxui::Color::RGB(255, 102, 102),
        .background = ftxui::Color::RGB(32, 32, 48),
        .text = ftxui::Color::RGB(221, 221, 255),
        .border = ftxui::Color::RGB(64, 64, 96)
    };

    // GitHub Theme
    themes_[Theme::GitHub] = {
        .primary = ftxui::Color::RGB(88, 166, 255),
        .secondary = ftxui::Color::RGB(121, 184, 255),
        .success = ftxui::Color::RGB(63, 185, 80),
        .warning = ftxui::Color::RGB(218, 165, 32),
        .error = ftxui::Color::RGB(248, 81, 73),
        .background = ftxui::Color::RGB(255, 255, 255),
        .text = ftxui::Color::RGB(36, 41, 46),
        .border = ftxui::Color::RGB(225, 228, 232)
    };

    // Material Theme
    themes_[Theme::Material] = {
        .primary = ftxui::Color::RGB(33, 150, 243),
        .secondary = ftxui::Color::RGB(0, 188, 212),
        .success = ftxui::Color::RGB(76, 175, 80),
        .warning = ftxui::Color::RGB(255, 193, 7),
        .error = ftxui::Color::RGB(244, 67, 54),
        .background = ftxui::Color::RGB(250, 250, 250),
        .text = ftxui::Color::RGB(33, 33, 33),
        .border = ftxui::Color::RGB(224, 224, 224)
    };

    // MaterialDark Theme
    themes_[Theme::MaterialDark] = {
        .primary = ftxui::Color::RGB(144, 202, 249),
        .secondary = ftxui::Color::RGB(128, 222, 234),
        .success = ftxui::Color::RGB(165, 214, 167),
        .warning = ftxui::Color::RGB(255, 224, 130),
        .error = ftxui::Color::RGB(239, 154, 154),
        .background = ftxui::Color::RGB(18, 18, 18),
        .text = ftxui::Color::RGB(255, 255, 255),
        .border = ftxui::Color::RGB(66, 66, 66)
    };

    // MaterialLight Theme
    themes_[Theme::MaterialLight] = {
        .primary = ftxui::Color::RGB(3, 169, 244),
        .secondary = ftxui::Color::RGB(0, 188, 212),
        .success = ftxui::Color::RGB(76, 175, 80),
        .warning = ftxui::Color::RGB(255, 193, 7),
        .error = ftxui::Color::RGB(211, 47, 47),
        .background = ftxui::Color::RGB(255, 255, 255),
        .text = ftxui::Color::RGB(0, 0, 0),
        .border = ftxui::Color::RGB(224, 224, 224)
    };

    // MaterialBlue Theme
    themes_[Theme::MaterialBlue] = {
        .primary = ftxui::Color::RGB(66, 165, 245),
        .secondary = ftxui::Color::RGB(38, 198, 218),
        .success = ftxui::Color::RGB(102, 187, 106),
        .warning = ftxui::Color::RGB(255, 213, 79),
        .error = ftxui::Color::RGB(239, 83, 80),
        .background = ftxui::Color::RGB(232, 234, 246),
        .text = ftxui::Color::RGB(26, 26, 26),
        .border = ftxui::Color::RGB(207, 216, 220)
    };
}

} // namespace TUIKIT