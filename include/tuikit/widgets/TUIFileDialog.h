#ifndef TUIKIT_TUIFILEDIALOG_H
#define TUIKIT_TUIFILEDIALOG_H

#include "tuikit/core/TUIApp.h"
#include <functional>
#include <string>
#include <vector>

namespace TUIKIT {

enum class FileDialogMode {
    OpenFile,        ///< The selected file must already exist.
    SaveFile,        ///< A new name is accepted; overwrite asks for confirmation.
    SelectDirectory  ///< A directory is selected.
};

/// Full-screen modal file/folder picker built on TUIApp::show_modal.
///
/// Keyboard: Up/Down navigate, Enter accepts the selection (in directory
/// mode it selects the highlighted directory), Right opens the highlighted
/// directory, Backspace goes to the parent directory, Tab switches between
/// the list and the path text field, `h` toggles hidden files, Escape cancels.
/// Mouse: click to select, click the selected row again to activate,
/// OK/Cancel buttons, mouse wheel scrolling.
///
/// A leading "~" in the start directory or the typed path is expanded to the
/// home directory. The reported path is absolute and lexically normalized.
///
/// The callback receives the chosen path, or an empty string if cancelled.
class TUIFileDialog {
public:
    using Callback = std::function<void(const std::string& path)>;

    static void pickFile(TUIApp& app,
                         const std::string& start_dir = ".",
                         const std::vector<std::string>& filters = {},
                         FileDialogMode mode = FileDialogMode::OpenFile,
                         Callback callback = nullptr);
};

} // namespace TUIKIT

#endif // TUIKIT_TUIFILEDIALOG_H
