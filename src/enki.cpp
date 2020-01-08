#include <QApplication>
#include <QFile>
#include <QDebug>


#include "qutepart/qutepart.h"

#include "core.h"


int main(int argc, char** argv) {
    Q_INIT_RESOURCE(qutepart_syntax_files);
    QApplication app(argc, argv);

    Core core;
    for (int fileIndex = 1; fileIndex < argc; fileIndex++) {
        QString filePath = argv[fileIndex];
        core.workspace()->openFile(filePath);
    }

    core.mainWindow()->show();

    return app.exec();
}
