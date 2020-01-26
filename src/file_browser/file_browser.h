#pragma once

#include <QDockWidget>
#include <QMainWindow>


class FileBrowserDock: public QDockWidget {
public:
	FileBrowserDock(QMainWindow* mainWindow);
};