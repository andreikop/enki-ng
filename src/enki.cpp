#include <QApplication>
#include <QFile>
#include <QDebug>


#include "qutepart/qutepart.h"

#include "main_window.h"
#include "workspace.h"


int main(int argc, char** argv) {
    Q_INIT_RESOURCE(qutepart_syntax_files);
    QApplication app(argc, argv);

    MainWindow mainWindow;
    Workspace workspace(mainWindow);

    for (int fileIndex = 1; fileIndex < argc; fileIndex++) {
        QString filePath = argv[fileIndex];
        workspace.openFile(filePath);
    }

    mainWindow.show();

    return app.exec();
}
