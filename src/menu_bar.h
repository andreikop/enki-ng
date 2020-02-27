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
    QMenu* goToMenu() const;
    QMenu* editorMenu() const;

private:

    QAction* fileQuitAction_;
    QMenu* searchMenu_;
    QMenu* goToMenu_;
    QMenu* editorMenu_;
};
