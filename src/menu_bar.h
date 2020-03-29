#pragma once

#include <QMainWindow>
#include <QMenuBar>


class MenuBar: public QMenuBar {
public:
    MenuBar(QMainWindow* parent);

    QMenu* fileMenu() const;
    QMenu* viewMenu() const;

    QAction* fileOpenAction() const;
    QAction* fileSaveAction() const;
    QAction* fileCloseAction() const;

    QAction* switchDirectoryAction() const;

    QAction* viewPrevFileAction() const;
    QAction* viewNextFileAction() const;

    QMenu* searchMenu() const;
    QMenu* goToMenu() const;
    QMenu* editorMenu() const;

private:
    QMenu* fileMenu_;
    QMenu* viewMenu_;

    QAction* fileOpenAction_;
    QAction* fileSaveAction_;
    QAction* fileCloseAction_;

    QAction* switchDirectoryAction_;

    QAction* viewPrevFileAction_;
    QAction* viewNextFileAction_;

    QAction* fileQuitAction_;
    QMenu* searchMenu_;
    QMenu* goToMenu_;
    QMenu* editorMenu_;

    void initFileMenu();
    void initViewMenu();
    void initNavigationMenu();
};
