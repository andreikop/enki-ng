#include "core/core.h"
#include "core/main_window.h"

#include "lib/locator/locator_dialog.h"

#include "open_file_command.h"
#include "open_file.h"


OpenFile::OpenFile():
    showAction_(QIcon::fromTheme("go-jump"), "Go to file in project...", this)
{
    connect(&showAction_, &QAction::triggered, this, &OpenFile::onTriggered);
    showAction_.setShortcut(QKeySequence("Ctrl+P"));

    core().mainWindow().menuBar()->navigationMenu()->addAction(&showAction_);
}

void OpenFile::onTriggered() {
    LocatorDialog LocatorDialog(&core().mainWindow(), std::make_shared<OpenFileCommand>());
    LocatorDialog.exec();
}
