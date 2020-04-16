#include <QProcess>

#include "subprocess.h"


class Process: public QProcess {
public:
    Process(const QString& command, const QStringList& arguments) {
        connect(this, &Process::readyReadStandardOutput, this, &Process::readStdOut);
        connect(this, &Process::readyReadStandardError, this, &Process::readStdErr);
        start(command, arguments, QIODevice::ReadOnly);
    }

    SubProcessResult execute() {
        waitForFinished(1000);  // TODO configurable timeout?

        return SubProcessResult {
            exitCode(),
            QString::fromUtf8(stdOutBuffer_),  // TODO support other encodings
            QString::fromUtf8(stdErrBuffer_)  // TODO support other encodings
        };
    }

private slots:
    void readStdOut() {
        stdOutBuffer_.append(readAllStandardOutput());  // NOTE reallocating buffer, bad for huge outputs
    }

    void readStdErr() {
        stdErrBuffer_.append(readAllStandardError());  // NOTE reallocating buffer, bad for huge outputs
    }

private:
    QByteArray stdOutBuffer_;
    QByteArray stdErrBuffer_;
};


SubProcessResult runSubProcess(const QString& command, const QStringList& arguments) {
    Process process(command, arguments);
    return process.execute();
}
