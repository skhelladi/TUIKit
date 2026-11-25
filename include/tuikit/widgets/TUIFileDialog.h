#ifndef TUIKIT_TUIFILEDIALOG_H
#define TUIKIT_TUIFILEDIALOG_H

#include "tuikit/core/TUIApp.h"
#include <functional>
#include <memory>
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
/// The current directory is shown as a selectable list: directories first
/// ("📁 name/"), then files ("📄 name"), alphabetical. The highlighted entry
/// is inverted; Up/Down/PageUp/PageDown/Home/End navigate (keyboard or mouse
/// wheel), a mouse click selects an entry and clicking the selected entry
/// again activates it.
///
/// Keyboard (list focused): Enter accepts the selection (in directory mode it
/// selects the highlighted directory), Right opens the highlighted directory,
/// Backspace goes to the parent directory, `h` toggles hidden files.
/// Tab switches between the list and the path text field; while the path
/// field is focused, Enter accepts the typed path and Escape cancels.
///
/// A leading "~" in the start directory or the typed path is expanded to the
/// home directory. The reported path is absolute and lexically normalized.
///
/// The callback receives the chosen path, or an empty string if cancelled.
class TUIFileDialog {
public:
    using Callback = std::function<void(const std::string& path)>;

    TUIFileDialog(TUIApp& app,
                  const std::string& start_dir = ".",
                  const std::vector<std::string>& filters = {},
                  FileDialogMode mode = FileDialogMode::OpenFile);
    ~TUIFileDialog();

    /// Show the dialog as a modal overlay.
    void open();

    /// Absolute path of the currently highlighted entry in the list,
    /// or an empty string when no entry is selected.
    std::string selectedEntry() const;

    /// Convenience one-shot usage; the dialog lives until the callback fires.
    static void pickFile(TUIApp& app,
                         const std::string& start_dir = ".",
                         const std::vector<std::string>& filters = {},
                         FileDialogMode mode = FileDialogMode::OpenFile,
                         Callback callback = nullptr);

private:
    class FileDialogImpl;
    std::shared_ptr<FileDialogImpl> impl_;
};

} // namespace TUIKIT

#endif // TUIKIT_TUIFILEDIALOG_H
