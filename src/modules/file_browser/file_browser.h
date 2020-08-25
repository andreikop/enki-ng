#pragma once

#include <memory>

#include <QDockWidget>
#include <QMainWindow>

#include "core/module.h"


class FileBrowserDock: public QDockWidget {
    Q_OBJECT
public:
    FileBrowserDock(QMainWindow* mainWindow);
};

class FileBrowser: public Module {
public:
    FileBrowser();

private:
    std::unique_ptr<FileBrowserDock> dock_;
};
