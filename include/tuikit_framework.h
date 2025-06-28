#ifndef TUIKIT_FRAMEWORK_H
#define TUIKIT_FRAMEWORK_H

#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <map>
#include <optional> // For StyledButton's color_

namespace TUIKIT
{

    using namespace ftxui;

    // =============================================================================
    // TYPES AND UTILITIES
    // =============================================================================

    using EventCallback = std::function<void()>;
    using ValidationFunc = std::function<bool(const std::string &)>;

    enum class Theme
    {
        Default,
        Dark,
        Light,
        Monokai
    };

    struct AppTheme
    {
        Color primary = Color::Blue;
        Color secondary = Color::Cyan;
        Color success = Color::Green;
        Color warning = Color::Yellow;
        Color error = Color::Red;
        Color background = Color::Default;
        Color text = Color::Default;
        Color border = Color::White;
    };

    // =============================================================================
    // THEME MANAGER
    // =============================================================================

    class ThemeManager
    {
    public:
        static ThemeManager &instance();

        void set_theme(Theme theme);
        const AppTheme &get_theme() const;

    private:
        AppTheme current_theme_;
    };

    // =============================================================================
    // CUSTOM WIDGETS
    // =============================================================================

    class TUIKButton
    {
    public:
        TUIKButton(const std::string &text, EventCallback callback = nullptr);

        Component get_component();
        void set_style(Color color);
        Element render();

    private:
        std::string text_;
        EventCallback callback_;
        Component component_;
        std::optional<Color> color_;
    };

    class TUIKInput
    {
    public:
        TUIKInput(const std::string &placeholder = "", const std::string &label = "");

        Component get_component();
        void set_validator(ValidationFunc validator);
        std::string get_text() const;
        void set_text(const std::string &text);
        bool is_valid() const;
        Element render();

    private:
        std::string content_;
        std::string placeholder_;
        std::string label_;
        Component component_;
        ValidationFunc validator_;
    };

    // class PasswordInput {
    // public:
    //     PasswordInput(const std::string& placeholder = "", const std::string& label = "");
    //     Component get_component();
    //     std::string get_text() const;
    //     void set_text(const std::string& text);
    //     Element render();
    //
    // private:
    //     std::string content_;
    //     std::string placeholder_;
    //     std::string label_;
    //     Component component_;
    // };

    class TUIKTextArea
    {
    public:
        TUIKTextArea(const std::string &placeholder = "", const std::string &label = "", int height = 5);
        Component get_component();
        std::string get_text() const;
        void set_text(const std::string &text);
        Element render();

    private:
        std::string content_;
        std::string placeholder_;
        std::string label_;
        int height_;
        Component component_;
    };

    class StyledCheckbox
    {
    public:
        StyledCheckbox(const std::string &label, bool initial_state = false);

        Component get_component();
        bool is_checked() const;
        void set_checked(bool checked);
        Element render();

    private:
        std::string label_;
        bool checked_;
        Component component_;
    };

    class StyledRadiobox
    {
    public:
        StyledRadiobox(const std::vector<std::string> &options, int initial_selected = 0);

        Component get_component();
        int get_selected() const;
        std::string get_selected_text() const;
        Element render();

    private:
        std::vector<std::string> options_;
        int selected_;
        Component component_;
    };

    // class DatePicker {
    // public:
    //     DatePicker(int* year, int* month, int* day);
    //     Component get_component();
    //     Element render();
    //
    // private:
    //     int* year_;
    //     int* month_;
    //     int* day_;
    //     Component component_;
    // };

    struct TreeNode
    {
        std::string label;
        std::vector<TreeNode> children;
        bool is_expanded = false;
    };

    class TUIKTreeView
    {
    public:
        TUIKTreeView(TreeNode root_node);
        Component get_component();
        Element render();

    private:
        TreeNode root_node_;
        Component component_;

        Component build_tree_component(TreeNode &node);
    };

    class StyledMenu
    {
    public:
        StyledMenu(const std::vector<std::string> &options, EventCallback on_select = nullptr);

        Component get_component();
        int get_selected() const;
        std::string get_selected_text() const;
        void set_on_select(EventCallback callback);
        void set_options(const std::vector<std::string>& options);
        Element render();

    private:
        std::vector<std::string> options_;
        int selected_;
        Component component_;
        EventCallback on_select_;
    };

    // class StyledTable {
    // public:
    //     StyledTable(const std::vector<std::vector<std::string>>& data, const std::vector<std::string>& headers = {});
    //     Component get_component();
    //     Element render();
    //
    // private:
    //     std::vector<std::vector<std::string>> data_;
    //     std::vector<std::string> headers_;
    //     Component component_;
    // };

    class ProgressBar
    {
    public:
        ProgressBar(float initial_value = 0.0f, const std::string &label = "");

        void set_value(float value);
        float get_value() const;
        void set_label(const std::string &label);
        Element render();

    private:
        float value_;
        std::string label_;
    };

    // class ActivityIndicator {
    // public:
    //     ActivityIndicator();
    //     Component get_component();
    //     Element render();
    //
    // private:
    //     Component component_;
    // };

    class TUIKSlider
    {
    public:
        TUIKSlider(const std::string &label, float *value, float min = 0.0f, float max = 1.0f, float increment = 0.01f);
        Component get_component();
        Element render();

    private:
        std::string label_;
        float *value_;
        float min_;
        float max_;
        float increment_;
        Component component_;
    };

    // =============================================================================
    // CONTAINERS AND LAYOUTS
    // =============================================================================

    class TUIKPanel
    {
    public:
        TUIKPanel(const std::string &title = "", bool show_border = true);

        void add_child(Component child);
        void set_title(const std::string &title);
        Component get_component();

    private:
        std::string title_;
        bool show_border_;
        Component internal_container_;
        Component component_;
    };

    class TUIKScrollableContainer
    {
    public:
        TUIKScrollableContainer(Component content, int height);
        Component get_component();

    private:
        Component content_;
        int height_;
        Component component_;
    };

    class TUIKTabbedPane
    {
    public:
        TUIKTabbedPane(const std::vector<std::string> &tab_titles, const std::vector<Component> &tab_contents);
        Component get_component();
        int get_selected_tab() const;
        void set_selected_tab(int index);

    private:
        std::vector<std::string> tab_titles_;
        std::vector<Component> tab_contents_;
        int selected_tab_ = 0;
        Component component_;
    };

    class Application; // Forward declaration

    // =============================================================================
    // PREDEFINED DIALOGS (NON-BLOCKING)
    // =============================================================================

    class MessageDialog
    {
    public:
        static void show_info(Application &app, const std::string &message, const std::string &title = "Information");
        static void show_warning(Application &app, const std::string &message, const std::string &title = "Warning");
        static void show_error(Application &app, const std::string &message, const std::string &title = "Error");
        static void confirm(Application &app, const std::string &message, std::function<void(bool)> callback, const std::string &title = "Confirmation");
    };

    // =============================================================================
    // MAIN APPLICATION
    // =============================================================================

    class Application
    {
    public:
        Application(const std::string &title = "TUIKIT Application");

        void set_theme(Theme theme);
        void add_page(const std::string &name, Component page);
        void show_status_bar(bool show);
        void set_status_text(const std::string &status);
        void run();
        void exit();
        void show_modal(Component modal);
        void close_modal();
        void show_message(const std::string &message, const std::string &title = "");
        void show_error(const std::string &message, const std::string &title = "");
        void confirm(const std::string &message, std::function<void(bool)> callback, const std::string &title = "");
        void request_redraw(); // Force a redraw of the screen (useful after state modification in a callback)
        void PostEvent(ftxui::Event event);
        void set_active_page(int index);

    private:
        std::string title_;
        ScreenInteractive screen_;

        std::vector<std::string> page_names_;
        std::vector<Component> pages_;
        int active_page_ = 0;

        bool show_status_bar_ = false;
        std::string status_text_ = "Ready";

        Component modal_ = nullptr;
        bool show_modal_ = false;
    };

    class Notification
    {
    public:
        static void show(Application &app, const std::string &message, const std::string &title = "Notification", int duration_ms = 3000);
    };

    // =============================================================================
    // BUILDERS AND UTILITIES
    // =============================================================================

    class WidgetBuilder
    {
    public:
        static std::unique_ptr<TUIKButton> create_button(const std::string &text, EventCallback callback = nullptr);
        static std::unique_ptr<TUIKInput> create_input(const std::string &placeholder = "", const std::string &label = "");
        //    static std::unique_ptr<PasswordInput> create_password_input(const std::string& placeholder = "", const std::string& label = "");
        static std::unique_ptr<TUIKTextArea> create_text_area(const std::string &placeholder = "", const std::string &label = "", int height = 5);
        //    static std::unique_ptr<StyledTable> create_table(const std::vector<std::vector<std::string>>& data, const std::vector<std::string>& headers = {});
        static std::unique_ptr<TUIKTreeView> create_tree_view(TreeNode root_node);
        static std::unique_ptr<TUIKTabbedPane> create_tabbed_pane(const std::vector<std::string> &tab_titles, const std::vector<Component> &tab_contents);
        static std::unique_ptr<TUIKScrollableContainer> create_scrollable_container(Component content, int height);
        static std::unique_ptr<TUIKPanel> create_panel(const std::string &title = "", bool border = true);
        static Component create_form_field(const std::string &label, Component input);
        static Component create_button_bar(const std::vector<std::pair<std::string, EventCallback>> &buttons);
        static std::unique_ptr<TUIKSlider> create_slider(const std::string &label, float *value, float min = 0.0f, float max = 1.0f, float increment = 0.01f);
        //    static std::unique_ptr<ActivityIndicator> create_activity_indicator();
        //    static std::unique_ptr<DatePicker> create_date_picker(int* year, int* month, int* day);
    };

} // namespace TUIKIT

#endif // TUIKIT_FRAMEWORK_H
