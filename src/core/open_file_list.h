#pragma once

#include <QMainWindow>
#include <QDockWidget>

#include "workspace.h"

class OpenFileList: public QDockWidget {
public:
    OpenFileList(QMainWindow *mainWindow, Workspace *workspace);
};
