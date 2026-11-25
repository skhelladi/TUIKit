#ifndef TUIKIT_TUIICONS_H
#define TUIKIT_TUIICONS_H

#include <string>

namespace TUIKIT {

/// Unicode glyph catalog for widgets (buttons, toolbar items, tree nodes...).
/// Keep `const std::string` values so they can be concatenated cheaply.
namespace ICON {
    // General / application
    const std::string Rocket = "🚀";
    const std::string Home = "🏠";
    const std::string Settings = "⚙️";
    const std::string Search = "🔍";
    const std::string Clock = "🕐";
    const std::string Link = "🔗";
    const std::string Lock = "🔒";
    const std::string Unlock = "🔓";
    const std::string User = "👤";
    const std::string Terminal = "🖥️";
    const std::string Book = "📖";
    const std::string Help = "❓";

    // Files and edition
    const std::string NewFile = "📄";
    const std::string File = "📃";
    const std::string Folder = "📁";
    const std::string Open = "📂";
    const std::string Save = "💾";
    const std::string SaveAs = "💽";
    const std::string Edit = "✏️";
    const std::string Delete = "🗑️";
    const std::string Copy = "📋";
    const std::string Paste = "📌";
    const std::string Import = "📥";
    const std::string Export = "📤";

    // Execution controls
    const std::string Play = "▶️";
    const std::string Stop = "⏹️";
    const std::string Pause = "⏸️";
    const std::string Refresh = "🔄";
    const std::string Calculate = "🧮";
    const std::string Solver = "🌀";

    // Status / message severity
    const std::string Tasks = "✅";
    const std::string Quit = "❌";
    const std::string Close = "✖️";
    const std::string Info = "ℹ️";
    const std::string Warning = "⚠️";
    const std::string Error = "❗";
    const std::string Success = "✔️";

    // Navigation
    const std::string Collapsible = "↕️";
    const std::string ArrowUp = "⬆️";
    const std::string ArrowDown = "⬇️";
    const std::string ArrowLeft = "⬅️";
    const std::string ArrowRight = "➡️";
    const std::string ChevronUp = "▲";
    const std::string ChevronDown = "▼";
    const std::string ChevronLeft = "◀";
    const std::string ChevronRight = "▶";
    const std::string Back = "↩️";
    const std::string Forward = "↪️";
    const std::string Plus = "➕";
    const std::string Minus = "➖";

    // Project / engineering (CFD-oriented)
    const std::string ProjectTree = "🌳";
    const std::string Themes = "🎨";
    const std::string Chart = "📊";
    const std::string Domain = "🌐";
    const std::string Mesh = "🕸️";
    const std::string Boundary = "🧱";
}

} // namespace TUIKIT

#endif // TUIKIT_TUIICONS_H
