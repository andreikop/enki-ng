#include <QDebug>

#include "core/core.h"
#include "core/main_window.h"
#include "core/editor.h"
#include "core/workspace.h"

#include "zoom.h"


Zoom::Zoom() {
    QMenu* zoomMenu = core().mainWindow().menuBar()->zoomMenu();

    zoomInAction_ = std::shared_ptr<QAction>(zoomMenu->addAction(
        "Zoom In",
        this,
        &Zoom::onZoomIn,
        QKeySequence("Ctrl+=")));
    zoomInAction_->setEnabled(false);

    zoomOutAction_ = std::shared_ptr<QAction>(zoomMenu->addAction(
        "Zoom Out",
        this,
        &Zoom::onZoomOut,
        QKeySequence("Ctrl+-")));
    zoomOutAction_->setEnabled(false);

    connect(&core().workspace(), &Workspace::currentEditorChanged,
            this, &Zoom::onCurrentEditorChanged);
}

void Zoom::onZoomIn() {
    zoom(1.1);
}

void Zoom::onZoomOut() {
    zoom(1 / 1.1);
}

void Zoom::zoom(double multiplier) {
    Option<int>& fontSizeOption = Editor::getFontSizeOption();
    int fontSize = fontSizeOption.value();
    int step = int(double(fontSize) * (multiplier - 1));
    if (step == 0) {
        if (multiplier > 1) {
            step = 1;
        } else {
            step = -1;
        }
    }

    fontSize += step;
    // Set some reasonable ranges
    fontSize = std::max(4, fontSize);
    fontSize = std::min(256, fontSize);

    fontSizeOption.setValue(fontSize);

    for(Editor* editor: core().workspace().editors()) {
        QFont font = editor->qutepart().font();
        font.setPointSize(fontSize);
        editor->qutepart().setFont(font);
    }


}

void Zoom::onCurrentEditorChanged(Editor* editor) {
    zoomInAction_->setEnabled(editor != nullptr);
    zoomOutAction_->setEnabled(editor != nullptr);
}
