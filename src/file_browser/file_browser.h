#pragma once

#include <memory>

#include <QDockWidget>
#include <QMainWindow>

#include "module.h"


class FileBrowserDock: public QDockWidget {
public:
    FileBrowserDock(QMainWindow* mainWindow);
};

class FileBrowser: public Module {
public:
    FileBrowser();

private:
    std::unique_ptr<FileBrowserDock> dock_;
};