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
    fileCloseAction = addActionToMenu(fileMenu, "Close", QKeySequence::Close);

    fileMenu->addSeparator();

    m_fileQuitAction = fileMenu->addAction(
        "Quit",
        QApplication::instance(), SLOT(quit()),
        QKeySequence::Quit);

    viewPrevFileAction = addActionToMenu(viewMenu, "Previous file", QKeySequence("Ctrl+PgUp"));
    viewNextFileAction = addActionToMenu(viewMenu, "Next file", QKeySequence("Ctrl+PgDown"));

    QMenu* navigationMenu = addMenu("Navigation");
    searchMenu_ = navigationMenu->addMenu("Search & Replace");
}

QMenu* MenuBar::searchMenu() const {
    return searchMenu_;
}
