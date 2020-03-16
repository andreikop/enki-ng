#include <iostream>

#include <QApplication>
#include <QCommandLineParser>
#include <QFile>
#include <QDebug>


#include "qutepart/qutepart.h"

#include "core.h"

struct FileToOpen {
    QString path;
    int line;
};

struct CommandLine {
    CommandLine():
        debugLogs(false)
    {}

    QList<FileToOpen> existingFiles;
    QStringList notExistingFiles;
    bool debugLogs;
};


namespace {

const QCommandLineOption debugLogsOption ({"d", "debug"}, "Print debug logs.");

void initCommandLineParser(QCommandLineParser& parser) {
    parser.setApplicationDescription("A text editor for programmers. https://enki-editor.org");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOption(debugLogsOption);
}

CommandLine getCmdLineArguments(const QCommandLineParser& parser) {
    CommandLine result;

    for (const QString& arg: parser.positionalArguments()) {
        QFileInfo fInfo = QFileInfo(arg);
        if (fInfo.exists()) {
            result.existingFiles << FileToOpen{arg, -1};
        } else {
            result.notExistingFiles << arg;
        }
    }

    result.debugLogs = parser.isSet(debugLogsOption);

    return result;
}

void verboseLogHandler(QtMsgType /*type*/, const QMessageLogContext &/*context*/, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    std::cout << localMsg.constData() << std::endl;
    // TODO print context information. See qInstallMessageHandler function docs
}

void logHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    if (type > QtDebugMsg) {
        verboseLogHandler(type, context, msg);
    }
}

}  // anonymous namespace

int main(int argc, char** argv) {
    Q_INIT_RESOURCE(qutepart_syntax_files);

    QApplication app(argc, argv);
    app.setApplicationName("Enki Editor");
    app.setApplicationVersion("0.0.1");

    QCommandLineParser cmdLineParser;
    initCommandLineParser(cmdLineParser);
    cmdLineParser.process(app);
    CommandLine cmdLine = getCmdLineArguments(cmdLineParser);

    if (cmdLine.debugLogs) {
        qInstallMessageHandler(verboseLogHandler);
    } else {
        qInstallMessageHandler(logHandler);
    }

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
