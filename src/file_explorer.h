#pragma once

#include <QMainWindow>
#include <QDockWidget>



class FileExplorer: public QDockWidget {
public:
    FileExplorer(QMainWindow *mainWindow);
};
