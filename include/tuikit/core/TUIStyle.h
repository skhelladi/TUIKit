#ifndef TUIKIT_TUISTYLE_H
#define TUIKIT_TUISTYLE_H

#include <ftxui/screen/color.hpp>
#include <map>
#include <string>

namespace TUIKIT {

// Enum for predefined themes
enum class Theme {
    Default,
    Dark,
    Light,
    Monokai,
    Solarized,
    Dracula,
    Nord,
    Blueberry,
    GitHub,
    Material,
    MaterialDark,
    MaterialLight,
    MaterialBlue
};

// Structure to hold application theme colors
struct AppTheme {
    ftxui::Color primary = ftxui::Color::Blue;
    ftxui::Color secondary = ftxui::Color::Cyan;
    ftxui::Color success = ftxui::Color::Green;
    ftxui::Color warning = ftxui::Color::Yellow;
    ftxui::Color error = ftxui::Color::Red;
    ftxui::Color background = ftxui::Color::Default;
    ftxui::Color text = ftxui::Color::Default;
    ftxui::Color border = ftxui::Color::White;
};

// TUIStyle class to manage themes
class TUIStyle {
public:
    static TUIStyle& instance();

    void setGlobalTheme(Theme theme);
    const AppTheme& currentTheme() const;

private:
    TUIStyle(); // Private constructor for Singleton pattern
    AppTheme current_theme_;

    // Predefined themes
    std::map<Theme, AppTheme> themes_;

    void initializeThemes();
};

} // namespace TUIKIT

#endif // TUIKIT_TUISTYLE_H
