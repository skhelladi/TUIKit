#ifndef TUIKIT_TUISTYLE_H
#define TUIKIT_TUISTYLE_H

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <map>
#include <string>

namespace TUIKIT {

// Enum for predefined themes
enum class Theme {
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
    /// Background drawn behind editable fields (text fields, spin boxes, ...).
    ftxui::Color fieldBackground = ftxui::Color::GrayDark;
    /// Frame color of editable fields when they are not focused. A focused
    /// field uses the theme's primary color for its frame.
    ftxui::Color fieldBorder = ftxui::Color::GrayDark;
};

// TUIStyle class to manage themes
class TUIStyle {
public:
    static TUIStyle& instance();

    void setGlobalTheme(Theme theme);
    const AppTheme& currentTheme() const;

    /// Shared decoration for editable fields (text fields, text areas, spin
    /// boxes, combo boxes): subtle background plus a thin rounded frame,
    /// highlighted with the primary color while the field is focused. The
    /// decorated field expands horizontally (xflex).
    static ftxui::Component decorateField(ftxui::Component field);

    /// Element-level counterpart of decorateField(): draws the field
    /// background and frame around an already-rendered element.
    static ftxui::Element fieldFrame(ftxui::Element inner, bool focused);

private:
    TUIStyle(); // Private constructor for Singleton pattern
    AppTheme current_theme_;

    // Predefined themes
    std::map<Theme, AppTheme> themes_;

    void initializeThemes();
};

} // namespace TUIKIT

#endif // TUIKIT_TUISTYLE_H
