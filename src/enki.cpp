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

CommandLine getCmdLineArguments(bool &ok, const QCommandLineParser& parser) {
    CommandLine result;
    ok = false;

    QStringList posArgs = parser.positionalArguments();
    while ( ! posArgs.empty()) {
        QString arg = posArgs.takeFirst();
        QFileInfo fInfo = QFileInfo(arg);
        if (fInfo.exists()) {
            int line = -1;
            if ( (! posArgs.empty()) and posArgs[0].startsWith('+')) {
                QString lineArg = posArgs.takeFirst().mid(1);  // next argument without +
                bool convOk = false;
                line = int(lineArg.toLong(&convOk));
                if ( ! convOk) {
                    qCritical() << "For file" << arg << "specified incorrect line number" << lineArg;
                    return result;
                }
            }

            result.existingFiles << FileToOpen{arg, line};
        } else {
            result.notExistingFiles << arg;
        }
    }

    result.debugLogs = parser.isSet(debugLogsOption);

    ok = true;
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

    bool cmdLineOk = false;
    CommandLine cmdLine = getCmdLineArguments(cmdLineOk, cmdLineParser);
    if ( ! cmdLineOk) {
        return -1;
    }

    if (cmdLine.debugLogs) {
        qInstallMessageHandler(verboseLogHandler);
    } else {
        qInstallMessageHandler(logHandler);
    }

    Core core;

    for (const auto& f: cmdLine.existingFiles) {
        core.workspace().openFile(f.path, f.line);
    }

    for (const auto& path: cmdLine.notExistingFiles) {
        core.workspace().createEmptyNotSavedFile(path);
    }

    core.mainWindow().show();

    return app.exec();
}
