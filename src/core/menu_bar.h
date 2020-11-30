#pragma once

#include <QMainWindow>
#include <QMenuBar>


class MenuBar: public QMenuBar {
public:
    MenuBar(QMainWindow* parent);

    QMenu* fileMenu() const;
    QMenu* viewMenu() const;

    QAction* fileNewAction() const;
    QAction* fileOpenAction() const;
    QAction* fileSaveAction() const;
    QAction* fileCloseAction() const;

    QAction* switchDirectoryAction() const;
    QAction* recentDirsAction() const;

    QAction* viewPrevFileAction() const;
    QAction* viewNextFileAction() const;

    QMenu* navigationMenu() const;
    QMenu* searchMenu() const;
    QMenu* bookmarksMenu() const;
    QMenu* editorMenu() const;
    QMenu* zoomMenu() const;

private:
    QMenu* fileMenu_;
    QMenu* viewMenu_;
    QMenu* zoomMenu_;

    QAction* fileNewAction_;
    QAction* fileOpenAction_;
    QAction* fileSaveAction_;
    QAction* fileCloseAction_;

    QAction* switchDirectoryAction_;
    QAction* recentDirsAction_;

    QAction* viewPrevFileAction_;
    QAction* viewNextFileAction_;

    QAction* fileQuitAction_;
    QMenu* navigationMenu_;
    QMenu* searchMenu_;
    QMenu* bookmarksMenu_;
    QMenu* editorMenu_;

    void initFileMenu();
    void initViewMenu();
    void initNavigationMenu();
};
