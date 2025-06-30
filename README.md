# TUIKit: A Qt-like Terminal User Interface Framework for C++

## 🚀 Project Overview

TUIKit is a modern C++ framework designed to build rich and interactive Terminal User Interfaces (TUIs). Inspired by the simplicity and power of the Qt framework, TUIKit aims to provide a familiar development experience for creating command-line applications, especially suited for scientific and engineering projects.

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

## 🛠️ Getting Started

### Prerequisites

*   CMake (version 3.15 or higher)
*   A C++17 compatible compiler (e.g., Clang, GCC)

### Building the Project

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/your-repo/TUIKit.git # Replace with your actual repo URL
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

## 🧪 Usage Example (Simplified)

Here's a minimal example demonstrating how to create a simple TUI application with TUIKit:

```cpp
#include "tuikit.h"
#include <memory>

int main() {
    TUIKIT::TUIApp app("My Simple TUI App");

    auto label = std::make_shared<TUIKIT::TUILabel>("Hello from TUIKit!");
    auto button = std::make_shared<TUIKIT::TUIButton>("Click Me!");
    button->onClick([&app]() {
        app.show_message("Button Clicked!", "Info");
    });

    auto main_layout = std::make_shared<TUIKIT::TUIVBoxLayout>();
    main_layout->addWidget(label);
    main_layout->addWidget(button);

    app.setMainWidget(main_layout);
    return app.exec();
}
```

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

## 🤝 Contributing

Contributions are welcome! Please refer to the `CONTRIBUTING.md` file (to be created) for guidelines on how to contribute to TUIKit.

## 📄 License

This project is licensed under the MIT License - see the `LICENSE` file for details.
