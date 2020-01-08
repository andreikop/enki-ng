#pragma once

#include <QMainWindow>
#include <QMenuBar>


class MenuBar: public QMenuBar {
public:
    MenuBar(QMainWindow* parent);

    QMenu* fileMenu;
    QMenu* viewMenu;

    QAction* fileOpenAction;
    QAction* fileSaveAction;
    QAction* fileCloseAction;

    QAction* viewPrevFileAction;
    QAction* viewNextFileAction;

    QMenu* searchMenu() const;

private:

    QAction* m_fileQuitAction;
    QMenu* searchMenu_;
};
