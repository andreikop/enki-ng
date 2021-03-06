#include <QFileDialog>

#include "core/core.h"
#include "core/project.h"
#include "core/main_window.h"
#include "core/workspace.h"

#include "basic_actions.h"


BasicActions::BasicActions() {
    MainWindow& mainWindow = core().mainWindow();
    MenuBar* menuBar = mainWindow.menuBar();

    connect(
        mainWindow.menuBar()->switchDirectoryAction(), &QAction::triggered,
        this, &BasicActions::onSwitchDirectoryAction);

    connect(menuBar->fileNewAction(), &QAction::triggered, this, &BasicActions::onFileNew);
    connect(menuBar->fileOpenAction(), &QAction::triggered, this, &BasicActions::onFileOpen);
    connect(menuBar->fileSaveAction(), &QAction::triggered, this, &BasicActions::onFileSave);

    Workspace* workspace = &core().workspace();
    connect(menuBar->fileCloseAction(), &QAction::triggered, workspace, &Workspace::closeCurrentFile);

    connect(menuBar->viewNextFileAction(), &QAction::triggered, workspace, &Workspace::activateNextFile);
    connect(menuBar->viewPrevFileAction(), &QAction::triggered, workspace, &Workspace::activatePrevFile);

    connect(workspace, &Workspace::currentEditorChanged, this, &BasicActions::updateEditMenuActions);
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

void BasicActions::onFileNew() {
    core().workspace().createNewFile();
}

void BasicActions::onFileOpen() {
    QStringList fileNames = QFileDialog::getOpenFileNames(
        &core().mainWindow(),
        "Open files",
        QDir::currentPath());

    foreach(const QString& fileName, fileNames) {
        core().workspace().openFile(fileName);
    }
}

void BasicActions::onFileSave() {
    Editor* editor = core().workspace().currentEditor();
    if (editor == nullptr) {
        return;
    }

    editor->saveFile();
}

void BasicActions::updateEditMenuActions(Editor* editor) {
    QMenu* editorMenu = core().mainWindow().menuBar()->editorMenu();
    editorMenu->clear();
    QMenu* bookmarksMenu = core().mainWindow().menuBar()->bookmarksMenu();
    bookmarksMenu->clear();

    if (editor != nullptr) {
        Qutepart::Qutepart& qpart = editor->qutepart();

        QMenu* scrollingMenu = editorMenu->addMenu("Scrolling");
        scrollingMenu->addAction(qpart.scrollUpAction());
        scrollingMenu->addAction(qpart.scrollDownAction());

        editorMenu->addAction(qpart.invokeCompletionAction());
        editorMenu->addAction(qpart.duplicateSelectionAction());

        QMenu* editLinesMenu = editorMenu->addMenu("Lines");
        editLinesMenu->addAction(qpart.deleteLineAction());
        editLinesMenu->addAction(qpart.moveLineUpAction());
        editLinesMenu->addAction(qpart.moveLineDownAction());
        editLinesMenu->addSeparator();
        editLinesMenu->addAction(qpart.cutLineAction());
        editLinesMenu->addAction(qpart.copyLineAction());
        editLinesMenu->addAction(qpart.pasteLineAction());
        editLinesMenu->addSeparator();
        editLinesMenu->addAction(qpart.joinLinesAction());
        editLinesMenu->addSeparator();
        editLinesMenu->addAction(qpart.insertLineAboveAction());
        editLinesMenu->addAction(qpart.insertLineBelowAction());

        QMenu* indentMenu = editorMenu->addMenu("Indentation");
        indentMenu->addAction(qpart.increaseIndentAction());
        indentMenu->addAction(qpart.decreaseIndentAction());

        bookmarksMenu->addAction(qpart.toggleBookmarkAction());
        bookmarksMenu->addAction(qpart.prevBookmarkAction());
        bookmarksMenu->addAction(qpart.nextBookmarkAction());
    }
}
