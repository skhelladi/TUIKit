# TUIKit - A Modern TUI Framework for C++

TUIKit is a lightweight and modern framework designed to simplify the creation of Text-based User Interface (TUI) applications in C++. It is built on top of the powerful [FTXUI](https://github.com/ArthurSonzogni/ftxui) library and provides a higher-level API, inspired by graphical UI frameworks.

This project serves as a template and example for building complex and interactive applications directly in your terminal.

## Features

*   **Component-Oriented API:** Build interfaces by assembling reusable widgets (`TUIKButton`, `TUIKMenu`, `TUIKInput`, `TUIKPanel`, `TUIKSlider`, `TUIKTextArea`, `TUIKTreeView`, `TUIKTabbedPane`, `TUIKScrollableContainer`, etc.).
*   **Flexible Layouts:** Organize your widgets vertically, horizontally, or in grids.
*   **Intuitive Event Handling:** Use lambdas for simple and direct callbacks.
*   **Theming System:** Easily customize the appearance of your application.
*   **Integrated Dialogs & Notifications:** Display dialog boxes for information, warnings, errors, or confirmations, and non-modal notifications.
*   **Simplified Build:** The CMake build system automatically handles downloading the FTXUI dependency.

## Implemented Components

Here's a list of the currently implemented components:

*   `Application`: The core class orchestrating the application.
*   `ThemeManager`: Manages application themes.
*   `MessageDialog`: For modal dialogs (info, warning, error, confirm).
*   `Notification`: For non-modal, temporary messages.
*   `WidgetBuilder`: A utility class for creating widget instances.
*   `TUIKButton`: A styled button.
*   `TUIKInput`: A styled text input field with validation.
*   `TUIKTextArea`: A multi-line text input field.
*   `TUIKCheckbox`: A styled checkbox.
*   `TUIKRadiobox`: A styled radio button group.
*   `TUIKMenu`: A styled selectable menu.
*   `ProgressBar`: A visual progress bar.
*   `TUIKSlider`: A component for selecting a value within a range.
*   `TUIKPanel`: A container with a title and border.
*   `TUIKTreeView`: For displaying hierarchical data.
*   `TUIKTabbedPane`: A container with tabs to organize content.
*   `TUIKScrollableContainer`: A container that allows scrolling its content.

## Planned/Future Components (Currently Commented Out)

Some components are planned but currently commented out due to compatibility or implementation details with the current FTXUI version (v5.0.0). These include:

*   `TUIKTable` (FTXUI `Table` component not directly available/compatible)
*   `TUIKActivityIndicator` (FTXUI `Spinner` component not directly available/compatible)
*   `TUIKDatePicker` (FTXUI `DatePicker` component not directly available/compatible)
*   `TUIKPasswordInput` (FTXUI `InputOption::Password()` not directly available/compatible)

## Project Structure

```
.
├── CMakeLists.txt        # Build configuration file
├── example_usage.cpp     # A comprehensive example showcasing various features
├── include/
│   └── tuikit_framework.h # The main framework header
├── src/
│   └── tuikit_framework.cpp # The main framework implementation
└── build/                  # Created by CMake, contains binaries
```

## Dependencies

*   **C++17** (or newer)
*   **CMake** (version 3.15 or newer)
*   **FTXUI** (automatically downloaded by CMake)

## Compilation

The following instructions will guide you through compiling the example application.

```bash
# 1. Clone the repository (if you haven't already)
# git clone https://github.com/skhelladi/TUIKit.git
# cd TUIKit

# 2. Create a build directory
mkdir -p build
cd build

# 3. Configure the project with CMake
cmake ..

# 4. Compile the project
make

# The executable will be in the build directory
# ./example_usage
```

## Basic Usage

The framework revolves around the `Application` class.

1.  **Create an `Application` instance**.
2.  **Add pages to the application** using `app.add_page(name, component)`.
3.  **Run the application** with `app.run()`.

```cpp
#include "tuikit_framework.h"

int main() {
    TUIKIT::Application app("My App");

    // Create a simple page
    auto my_page = ftxui::Renderer([] {
        return ftxui::text("Hello, TUIKIT!") | ftxui::border;
    });

    app.add_page("Home", my_page);
    app.run();

    return 0;
}
```

## Author

*   **Sofiane KHELLADI** ([sofiane@khelladi.page](mailto:sofiane@khelladi.page))

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
