#include <QFont>
#include <QFileInfo>

#include "core.h"
#include "editor.h"
#include "project.h"
#include "workspace.h"

#include "main_window.h"

MainWindow::MainWindow() {
    setMenuBar(new MenuBar(this));
    resize(800, 600);

    QFont font = this->font();
    font.setPointSize(12);
    font.setFamily("Monospace");
    setFont(font);

    QWidget* widget = new QWidget(this);
    centralLayout_ = new QVBoxLayout(widget);
    centralLayout_->setContentsMargins(0, 0, 0, 0);
    setCentralWidget(widget);
    setStyleSheet("QMainWindow::separator{width: 4px}");

    connect(&core().project(), &Project::pathChanged, this, &MainWindow::updateTitle);
}

MenuBar* MainWindow::menuBar() {
    return dynamic_cast<MenuBar*>(QMainWindow::menuBar());
}


void MainWindow::setWorkspace(QWidget* workspace) {
    centralLayout_->addWidget(workspace);
}

void MainWindow::setSearchWidget(QWidget* searchWidget) {
    centralLayout_->addWidget(searchWidget);
}

void MainWindow::updateTitle() {
    Editor* editor = core().workspace().currentEditor();

    if (editor != nullptr) {
        QDir projDir = core().project().path();

        setWindowTitle(
            QString("%1 - %2[*]")
                .arg(projDir.dirName())
                .arg(projDir.relativeFilePath(editor->filePath()))
            );

        setWindowModified(editor->qutepart().document()->isModified());
    } else {
        setWindowTitle(core().project().path().path());
        setWindowModified(false);
    }
}
