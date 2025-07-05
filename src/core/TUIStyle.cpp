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
        ftxui::Color::Blue,
        ftxui::Color::Cyan,
        ftxui::Color::Green,
        ftxui::Color::Yellow,
        ftxui::Color::Red,
        ftxui::Color::Black,
        ftxui::Color::White,
        ftxui::Color::GrayDark
    };

    // Light Theme
    themes_[Theme::Light] = {
        ftxui::Color::Blue,
        ftxui::Color::Cyan,
        ftxui::Color::Green,
        ftxui::Color::Yellow,
        ftxui::Color::Red,
        ftxui::Color::White,
        ftxui::Color::Black,
        ftxui::Color::GrayLight
    };
    
    // Monokai Theme
    themes_[Theme::Monokai] = {
        ftxui::Color::Magenta,
        ftxui::Color::Cyan,
        ftxui::Color::GreenLight,
        ftxui::Color::Yellow,
        ftxui::Color::RedLight,
        ftxui::Color::Black,
        ftxui::Color::White,
        ftxui::Color::GrayDark
    };

    // Solarized Theme
    themes_[Theme::Solarized] = {
        ftxui::Color::Blue,
        ftxui::Color::Cyan,
        ftxui::Color::Green,
        ftxui::Color::Yellow,
        ftxui::Color::Red,
        ftxui::Color::RGB(0, 43, 54),
        ftxui::Color::RGB(131, 148, 150),
        ftxui::Color::RGB(88, 110, 117)
    };

    // Dracula Theme
    themes_[Theme::Dracula] = {
        ftxui::Color::RGB(189, 147, 249),
        ftxui::Color::RGB(139, 233, 253),
        ftxui::Color::RGB(80, 250, 123),
        ftxui::Color::RGB(241, 250, 140),
        ftxui::Color::RGB(255, 85, 85),
        ftxui::Color::RGB(40, 42, 54),
        ftxui::Color::RGB(248, 248, 242),
        ftxui::Color::RGB(98, 114, 164)
    };

    // Nord Theme
    themes_[Theme::Nord] = {
        ftxui::Color::RGB(136, 192, 208),
        ftxui::Color::RGB(129, 161, 193),
        ftxui::Color::RGB(163, 190, 140),
        ftxui::Color::RGB(235, 203, 139),
        ftxui::Color::RGB(191, 97, 106),
        ftxui::Color::RGB(46, 52, 64),
        ftxui::Color::RGB(216, 222, 233),
        ftxui::Color::RGB(76, 86, 106)
    };

    // Blueberry Theme
    themes_[Theme::Blueberry] = {
        ftxui::Color::RGB(130, 170, 255),
        ftxui::Color::RGB(137, 221, 255),
        ftxui::Color::RGB(199, 236, 127),
        ftxui::Color::RGB(255, 204, 102),
        ftxui::Color::RGB(255, 102, 102),
        ftxui::Color::RGB(32, 32, 48),
        ftxui::Color::RGB(221, 221, 255),
        ftxui::Color::RGB(64, 64, 96)
    };

    // GitHub Theme
    themes_[Theme::GitHub] = {
        ftxui::Color::RGB(88, 166, 255),
        ftxui::Color::RGB(121, 184, 255),
        ftxui::Color::RGB(63, 185, 80),
        ftxui::Color::RGB(218, 165, 32),
        ftxui::Color::RGB(248, 81, 73),
        ftxui::Color::RGB(255, 255, 255),
        ftxui::Color::RGB(36, 41, 46),
        ftxui::Color::RGB(225, 228, 232)
    };

    // Material Theme
    themes_[Theme::Material] = {
        ftxui::Color::RGB(33, 150, 243),
        ftxui::Color::RGB(0, 188, 212),
        ftxui::Color::RGB(76, 175, 80),
        ftxui::Color::RGB(255, 193, 7),
        ftxui::Color::RGB(244, 67, 54),
        ftxui::Color::RGB(250, 250, 250),
        ftxui::Color::RGB(33, 33, 33),
        ftxui::Color::RGB(224, 224, 224)
    };

    // MaterialDark Theme
    themes_[Theme::MaterialDark] = {
        ftxui::Color::RGB(144, 202, 249),
        ftxui::Color::RGB(128, 222, 234),
        ftxui::Color::RGB(165, 214, 167),
        ftxui::Color::RGB(255, 224, 130),
        ftxui::Color::RGB(239, 154, 154),
        ftxui::Color::RGB(18, 18, 18),
        ftxui::Color::RGB(255, 255, 255),
        ftxui::Color::RGB(66, 66, 66)
    };

    // MaterialLight Theme
    themes_[Theme::MaterialLight] = {
        ftxui::Color::RGB(3, 169, 244),
        ftxui::Color::RGB(0, 188, 212),
        ftxui::Color::RGB(76, 175, 80),
        ftxui::Color::RGB(255, 193, 7),
        ftxui::Color::RGB(211, 47, 47),
        ftxui::Color::RGB(255, 255, 255),
        ftxui::Color::RGB(0, 0, 0),
        ftxui::Color::RGB(224, 224, 224)
    };

    // MaterialBlue Theme
    themes_[Theme::MaterialBlue] = {
        ftxui::Color::RGB(66, 165, 245),
        ftxui::Color::RGB(38, 198, 218),
        ftxui::Color::RGB(102, 187, 106),
        ftxui::Color::RGB(255, 213, 79),
        ftxui::Color::RGB(239, 83, 80),
        ftxui::Color::RGB(232, 234, 246),
        ftxui::Color::RGB(26, 26, 26),
        ftxui::Color::RGB(207, 216, 220)
    };
}

} // namespace TUIKIT