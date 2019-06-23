#pragma once

#include <QMainWindow>
#include <QDockWidget>

#include "workspace.h"

class FileExplorer: public QDockWidget {
public:
    FileExplorer(QMainWindow *mainWindow, Workspace *workspace);
};
