# TUIKit: A Qt-like Terminal User Interface Framework for C++

## 🚀 Project Overview

Based on [FTXUI](https://github.com/ArthurSonzogni/FTXUI), TUIKit is a modern C++ framework designed to build rich and interactive Terminal User Interfaces (TUIs). Inspired by the simplicity and power of the Qt framework, TUIKit aims to provide a familiar development experience for creating command-line applications, especially suited for scientific and engineering projects.

<p align="center">
    <img src="docs/example_1.gif" alt="Demo of TUIKit in action"><br>
    <strong>See <code>example_1.cpp</code></strong><br>
</p>

## 🧩 Core Principles (Qt-like Philosophy)

*   **Hierarchical Widgets:** Utilize a structured hierarchy of UI components (`TUIWidget`, `TUIForm`, `TUIMenu`, etc.).
*   **Event Handling:** Manage user interactions through a signal/slot-like mechanism or callbacks.
*   **Automatic Layouts:** Employ flexible layout managers (`TUIVBox`, `TUIHBox`) for responsive UI design.
*   **Advanced Theming:** Customize colors, borders, and styles to create visually appealing interfaces.
*  **UI Definition Files:** Support for defining UI layouts in JSON-like `.tuik` files, allowing dynamic loading and modification of interfaces.

## ✨ Implemented Components (Current State)

The following core components are implemented and functional:

*   `TUIApp`: The entry point for the application.
*   `TUIWidget`: Base class for all UI components.
*   `TUIButton`: Interactive button with click events and icon support.
*   `TUICheckBox`: Simple toggle checkbox.
*   `TUICollapsible`: A collapsible container for widgets.
*   `TUIComboBox`: Dropdown list for selection.
*   `TUIDoubleSpinBox`: Spin box for double-precision floating-point values.
*   `TUIForm`: Organizes input fields with labels.
*   `TUIGroupBox`: Groups related widgets with an optional title and border.
*   `TUILabel`: Displays static text.
*   `TUIListView`: A simple list of selectable string items.
*   `TUIMenu`: Vertical list of selectable options.
*   `TUIProgressBar`: A widget to show progress.
*   `TUIRadioBox`: Group of exclusive radio buttons.
*   `TUIResizableSplit`: Splitter for resizable panels (horizontal/vertical).
*   `TUIScrollableContainer`: Scrollable area for any widget (with scrollbar, keyboard and mouse support).
*   `TUISlider`: Horizontal slider for continuous value selection.
*   `TUISpinBox`: Spin box for integer values.
*   `TUIStatusBar`: Displays status messages at the bottom of the application.
*   `TUITableView`: A widget for displaying data in a table.
*   `TUITabWidget`: Organizes content into multiple tabs.
*   `TUITextArea`: A multi-line text editing widget.
*   `TUITextField`: Editable single-line text input.
*   `TUIToolbar`: A bar for action buttons.
*   `TUITreeView`: Displays hierarchical data in a tree structure.

## 🛠️ Getting Started

### Prerequisites

*   **C++17/20 Compiler**: A compiler that supports C++17/20 (e.g., GCC, Clang, MSVC).
*   **CMake**: Version 3.15 or higher.
*   **FTXUI**: A terminal UI library that TUIKit is built upon.
*   **nlohmann/json**: For JSON parsing, used in `.tuik` file handling.

### Building the Project

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/skhelladi/TUIKit.git
    cd TUIKit
    ```
2.  **Install dependencies:**
    - Install FTXUI:
    You need to clone it into the `external/ftxui` directory:
    ```bash
    git clone https://github.com/ArthurSonzogni/FTXUI.git external/ftxui
    ```
    - Install nolmann/json for JSON parsing into `external/json`:
    ```bash
    wget https://github.com/nlohmann/json/releases/download/v3.12.0/json.hpp -P external/json
    ```

3.  **Create a build directory and configure CMake:**
    ```bash
    mkdir build
    cd build
    cmake ..
    ```

4.  **Build the project:**
    ```bash
    cmake --build .
    ```

### Running the Example

After a successful build, you can run the examples provided in the `examples` directory. The build process will generate executables in the `build` directory.:

```bash
./build/example_1
```
and 
```bash
./build/example_2
```

## 🧪 Usage Example

Here's a minimal example demonstrating how to create a simple TUI application with TUIKit, showcasing factory functions and the `connect` mechanism for event handling.

```cpp
#include "tuikit.h"

using namespace TUIKIT;

int main() {
    TUIApp app("My Simple TUI App");

    auto main_layout = vbox();
    
    auto my_label = label("Hello from TUIKit!");
    main_layout->addWidget(my_label);

    auto my_button = button("Click Me!");
    connect(my_button, [&] {
        my_label->setText("Button Clicked!");
    });
    
    // or you can use a lambda function directly
    // my_button->onClick([]() {
    //     my_label->setText("Button Clicked!");
    // });

    main_layout->addWidget(my_button);

    app.setMainWidget(main_layout);
    return app.exec();
}
```

---

## 🆕 Example: Embedding a `.tuik` UI file

TUIKit supports embedding JSON UI definitions (`.tuik` files) directly into your binary using CMake and `xxd`.  
For each `.tuik` file listed in `TUIKIT_UI`, a header `<name>_tuik.h` is generated with a binary variable.

**Example `.tuik` file (`examples/simple_ui.tuik`):**
```json
{
  "class": "TUIVBoxLayout",
  "children": [
    { "class": "TUILabel", "properties": { "text": "Hello from .tuik!" } },
    { "class": "TUIButton", "properties": { "text": "Click Me" }, "name": "btn" }
  ]
}
```

**C++ usage:**
```cpp
#include "tuikit.h"
#include "examples/simple_ui_tuik.h"
#include <nlohmann/json.hpp>

using namespace TUIKIT;

int main() {
    TUIApp app("TUI .tuik Example");

    // Load UI from embedded binary resource
    std::string tuik_str(reinterpret_cast<const char*>(simple_ui_tuik), simple_ui_tuik_len);
    nlohmann::json tuik_json = nlohmann::json::parse(tuik_str);

    TUIKLoader loader;
    auto main_ui_widget = loader.createWidgetFromJson(tuik_json);
    app.setMainWidget(main_ui_widget);

    // Connect a slot to the button named "btn"
    auto btn = std::dynamic_pointer_cast<TUIButton>(loader.getWidget("btn"));
    if (btn) {
        connect(btn, [&] { /* action */ });
    }

    return app.exec();
}
```

**CMake integration:**
```cmake
find_program(XXD_EXE xxd REQUIRED)
set(TUIKIT_UI "examples/simple_ui.tuik")
foreach(TUIK_FILE ${TUIKIT_UI})
    get_filename_component(TUIK_NAME ${TUIK_FILE} NAME_WE)
    get_filename_component(TUIK_DIR ${TUIK_FILE} DIRECTORY)
    set(VAR_NAME "${TUIK_NAME}_tuik")
    set(HEADER_FILE "${TUIK_DIR}/${TUIK_NAME}_tuik.h")
    add_custom_command(
        OUTPUT ${HEADER_FILE}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${TUIK_DIR}
        COMMAND ${XXD_EXE} -i -n ${VAR_NAME} ${TUIK_FILE} > ${HEADER_FILE}
        DEPENDS ${TUIK_FILE}
        COMMENT "Generate ${HEADER_FILE} from ${TUIK_FILE} with variable ${VAR_NAME} (xxd required)"
        VERBATIM
    )
    list(APPEND EMBEDDED_HEADERS ${HEADER_FILE})
endforeach()
add_custom_target(generate_embedded_headers ALL DEPENDS ${EMBEDDED_HEADERS})
```
<!-- 
## 🗺️ Development Roadmap

TUIKit is under active development, with future phases planned to enhance its capabilities:

### Phase 1: Foundation (MVP) - *Mostly Complete*
*   CMake template + basic structure
*   `TUIApp`, `TUIWidget` (base classes)
*   `TUIMenu`, `TUIForm`, `TUILabel`, `TUITextField`
*   Basic Layouts (`TUIVBox`, `TUIHBox`)
*   Simple Styling System

### Phase 2: Enriched Widgets - *In Progress*
*   `TUIComboBox`, `TUISpinBox`, `TUICheckBox`
*   `TUIGroupBox` for sections
*   `TUIToolbar` with action buttons
*   First version of `TUIBarChart`

### Phase 3: Advanced Visualization
*   `TUILineChart`, `TUIHistogram`, `TUITable`
*   `TUITree` for hierarchical navigation
*   `TUITabWidget` for tabbed organization
*   Advanced Theming System

### Phase 4: Interactivity and Data Management
*   JSON ↔ UI binding system
*   `TUIProjectManager` for multiple projects
*   Real-time field validation
*   Multi-threading for long-running tasks

### Phase 5: Scientific Specialization
*   Specialized CFD/FEA widgets
*   Plugin system
*   Integration with external solvers
*   Configuration export/import
-->
## 🤝 Contributing

Contributions are welcome! Please refer to the `CONTRIBUTING.md` file (to be created) for guidelines on how to contribute to TUIKit.

## 📄 License

This project is licensed under the MIT License - see the `LICENSE` file for details.
