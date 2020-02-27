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
    fileMenu(addMenu("File")),
    viewMenu(addMenu("View"))
{
    fileOpenAction = addActionToMenu(fileMenu, "Open...", QKeySequence::Open);
    fileSaveAction = addActionToMenu(fileMenu, "Save", QKeySequence::Save);

    fileCloseAction = addActionToMenu(fileMenu, "Close", QKeySequence("Ctrl+W"));

    // Workaround a bug on some systems. Alt+F4 is a bad shortcut to close document
    if (fileCloseAction->shortcut() == QKeySequence("Alt+F4")) {
        fileCloseAction->setShortcut(QKeySequence("Ctrl+W"));
    }

    fileMenu->addSeparator();

    fileQuitAction_ = fileMenu->addAction(
        "Quit",
        QApplication::instance(), SLOT(quit()),
        QKeySequence::Quit);

    viewPrevFileAction = addActionToMenu(viewMenu, "Previous file", QKeySequence("Ctrl+PgUp"));
    viewNextFileAction = addActionToMenu(viewMenu, "Next file", QKeySequence("Ctrl+PgDown"));

    QMenu* navigationMenu = addMenu("Navigation");
    searchMenu_ = navigationMenu->addMenu("Search and Replace");

    editorMenu_ = addMenu("Editor");

    goToMenu_ = addMenu("Go To");
}

QMenu* MenuBar::searchMenu() const {
    return searchMenu_;
}

QMenu* MenuBar::goToMenu() const {
    return goToMenu_;
}

QMenu* MenuBar::editorMenu() const {
    return editorMenu_;
}
