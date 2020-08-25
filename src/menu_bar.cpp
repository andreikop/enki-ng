#include <QApplication>

#include "menu_bar.h"


namespace {
QAction* addActionToMenu(QMenu* menu, const QString& text, const QKeySequence& shortcut) {
    QAction* action = menu->addAction(text);
    action->setShortcut(shortcut);
    return action;
}
}

MenuBar::MenuBar(QMainWindow* parent):
    QMenuBar(parent),
    fileMenu_(addMenu("File")),
    viewMenu_(addMenu("View"))
{
    initFileMenu();
    initViewMenu();
    initNavigationMenu();

    editorMenu_ = addMenu("Editor");
}

QMenu* MenuBar::fileMenu() const {
    return fileMenu_;
}

QMenu* MenuBar::viewMenu() const {
    return viewMenu_;
}

QMenu* MenuBar::zoomMenu() const {
    return zoomMenu_;
}

QAction* MenuBar::fileOpenAction() const {
    return fileOpenAction_;
}

QAction* MenuBar::fileSaveAction() const {
    return fileSaveAction_;
}

QAction* MenuBar::fileCloseAction() const {
    return fileCloseAction_;
}

QAction* MenuBar::switchDirectoryAction() const {
    return switchDirectoryAction_;
}

QAction* MenuBar::viewPrevFileAction() const {
    return viewPrevFileAction_;
}

QAction* MenuBar::viewNextFileAction() const {
    return viewNextFileAction_;
}

QMenu* MenuBar::searchMenu() const {
    return searchMenu_;
}

QMenu* MenuBar::navigationMenu() const {
    return navigationMenu_;
}

QMenu* MenuBar::bookmarksMenu() const {
    return bookmarksMenu_;
}

QMenu* MenuBar::editorMenu() const {
    return editorMenu_;
}

void MenuBar::initFileMenu() {
    fileOpenAction_ = addActionToMenu(fileMenu_, "Open...", QKeySequence::Open);
    fileSaveAction_ = addActionToMenu(fileMenu_, "Save", QKeySequence::Save);

    fileCloseAction_ = addActionToMenu(fileMenu_, "Close", QKeySequence("Ctrl+W"));

    // Workaround a bug on some systems. Alt+F4 is a bad shortcut to close document
    if (fileCloseAction_->shortcut() == QKeySequence("Alt+F4")) {
        fileCloseAction_->setShortcut(QKeySequence("Ctrl+W"));
    }

    fileMenu_->addSeparator();

    switchDirectoryAction_ = addActionToMenu(
        fileMenu_, "Switch directory...", QKeySequence("Shift+Ctrl+O"));

    fileMenu_->addSeparator();

    fileQuitAction_ = fileMenu_->addAction(
        "Quit",
        QApplication::instance(), SLOT(quit()),
        QKeySequence::Quit);
}

void MenuBar::initViewMenu() {
    viewPrevFileAction_ = addActionToMenu(viewMenu_, "Previous file", QKeySequence("Ctrl+PgUp"));
    viewNextFileAction_ = addActionToMenu(viewMenu_, "Next file", QKeySequence("Ctrl+PgDown"));
    zoomMenu_ = viewMenu_->addMenu("Zoom");
}

void MenuBar::initNavigationMenu() {
    navigationMenu_ = addMenu("Navigation");
    searchMenu_ = navigationMenu_->addMenu("Search and Replace");
    bookmarksMenu_ = navigationMenu_->addMenu("Boomarks");
}
