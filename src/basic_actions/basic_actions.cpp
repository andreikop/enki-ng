#include <QFileDialog>

#include "core.h"
#include "project.h"
#include "main_window.h"

#include "basic_actions.h"


BasicActions::BasicActions() {
    connect(
        core().mainWindow().menuBar()->switchDirectoryAction(), &QAction::triggered,
        this, &BasicActions::onSwitchDirectoryAction);
}

void BasicActions::onSwitchDirectoryAction() {
    QString path = QFileDialog::getExistingDirectory(
        &core().mainWindow(),
        "Switch current directory",
        core().project().path().path());

    if ( ! path.isNull()) {
        core().project().setPath(path);
    }
}
