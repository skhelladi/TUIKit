# TUIKit: A Qt-like Terminal User Interface Framework for C++

## 🚀 Project Overview

Based on [FTXUI](https://github.com/ArthurSonzogni/FTXUI), TUIKit is a modern C++ framework designed to build rich and interactive Terminal User Interfaces (TUIs). Inspired by the simplicity and power of the Qt framework, TUIKit aims to provide a familiar development experience for creating command-line applications, especially suited for scientific and engineering projects.

## 🧩 Core Principles (Qt-like Philosophy)

*   **Hierarchical Widgets:** Utilize a structured hierarchy of UI components (`TUIWidget`, `TUIForm`, `TUIMenu`, etc.).
*   **Event Handling:** Manage user interactions through a signal/slot-like mechanism or callbacks.
*   **Automatic Layouts:** Employ flexible layout managers (`TUIVBox`, `TUIHBox`) for responsive UI design.
*   **Advanced Theming:** Customize colors, borders, and styles to create visually appealing interfaces.

## ✨ Implemented Components (Current State)

Based on the `examples/main.cpp` and the current codebase, the following core components are implemented and functional:

*   `TUIApp`: The entry point for the application.
*   `TUIWidget`: Base class for all UI components.
*   `TUILabel`: Displays static text.
*   `TUITextField`: Editable single-line text input.
*   `TUIButton`: Interactive button with click events and icon support.
*   `TUIMenu`: Vertical list of selectable options.
*   `TUICheckBox`: Simple toggle checkbox.
*   `TUIRadioBox`: Group of exclusive radio buttons.
*   `TUIComboBox`: Dropdown list for selection.
*   `TUISlider`: Horizontal slider for continuous value selection.
*   `TUIToolbar`: A bar for action buttons.
*   `TUIForm`: Organizes input fields with labels.
*   `TUIGroupBox`: Groups related widgets with an optional title and border.
*   `TUITabWidget`: Organizes content into multiple tabs.
*   `TUIStatusBar`: Displays status messages at the bottom of the application.
*   `TUITreeView`: Displays hierarchical data in a tree structure.
*   `TUIResizableSplit`: Splitter for resizable panels (horizontal/vertical).
*   `TUIScrollableContainer`: Scrollable area for any widget (with scrollbar, keyboard and mouse support).

## 🛠️ Getting Started

### Prerequisites

*   **C++17 Compiler**: A compiler that supports C++17 (e.g., GCC, Clang, MSVC).
*   **CMake**: Version 3.15 or higher.
*   **FTXUI**: This project uses FTXUI as a dependency. You need to clone it into the `external/` directory:
    ```bash
    git clone https://github.com/ArthurSonzogni/FTXUI.git external/ftxui
    ```

### Building the Project

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/skhelladi/TUIKit.git
    cd TUIKit
    ```

2.  **Create a build directory and configure CMake:**
    ```bash
    mkdir build
    cd build
    cmake ..
    ```

3.  **Build the project:**
    ```bash
    cmake --build .
    ```

### Running the Example

After a successful build, you can run the example application:

```bash
./build/example_usage
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
