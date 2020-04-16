#pragma once

#include <QString>
#include <QStringList>

struct SubProcessResult {
    int exitCode;
    QString stdOut;
    QString stdErr;
};

SubProcessResult runSubProcess(const QString& command, const QStringList& arguments);
