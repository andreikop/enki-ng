#include <QApplication>
#include <QFile>
#include <QDebug>


#include "qutepart/qutepart.h"

#include "core.h"

struct FileToOpen {
    QString path;
    int line;
};

struct CommandLine {
    QList<FileToOpen> existingFiles;
    QStringList notExistingFiles;
};


namespace {

CommandLine parseCommandLine(int argc, char** argv) {
    CommandLine result;

    for (int fileIndex = 1; fileIndex < argc; fileIndex++) {
        QString filePath = argv[fileIndex];

        QFileInfo fInfo = QFileInfo(filePath);
        if (fInfo.exists()) {
            result.existingFiles << FileToOpen{filePath, -1};
        } else {
            result.notExistingFiles << filePath;
        }
    }

    return result;
}

}  // anonymous namespace

int main(int argc, char** argv) {
    Q_INIT_RESOURCE(qutepart_syntax_files);

    CommandLine cmdLine = parseCommandLine(argc, argv);

    QApplication app(argc, argv);

    Core core;

    for (const auto& f: cmdLine.existingFiles) {
        core.workspace().openFile(f.path);
    }

    for (const auto& path: cmdLine.notExistingFiles) {
        core.workspace().createEmptyNotSavedFile(path);
    }

    core.mainWindow().show();

    return app.exec();
}
