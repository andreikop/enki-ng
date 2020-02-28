#include <QApplication>
#include <QByteArray>
#include <QFile>
#include <QMessageBox>

#include "core.h"

#include "editor.h"


Editor::Editor(const QString& filePath, const QString& text, QMainWindow* parent):
    filePath_(filePath),
    qutepart_(parent, text)
{
    Qutepart::LangInfo langInfo = Qutepart::chooseLanguage(
        QString::null, QString::null, filePath);
    if (langInfo.isValid()) {
        qutepart_.setHighlighter(langInfo.id);
        qutepart_.setIndentAlgorithm(langInfo.indentAlg);
    }

    qutepart_.setFont(QFont(core().settings().fontFamily(), core().settings().fontSize()));
}

const QString& Editor::filePath() const {
    return filePath_;
}

Qutepart::Qutepart& Editor::qutepart() {
    return qutepart_;
}

void Editor::saveFile() {
    // assume file name is known
    // asume directory exists

    // TODO prepare text for saving. i.e. remove blank lines
    QByteArray data = qutepart_.document()->toPlainText().toUtf8();

    // TODO disable file watcher
    QFile file(filePath_);
    bool ok = file.open(QIODevice::WriteOnly);
    if ( ! ok) {
        QMessageBox::critical(
            qApp->activeWindow(),
            "Failed to save file",
            QString("Can not open file. Error: '%1'").arg(file.errorString()));
        return;
    }

    int written = file.write(data);

    if (written != data.length()) {
        QMessageBox::critical(
            qApp->activeWindow(),
            "Failed to save file",
            QString("%1 bytes written. Error: '%2'").arg(QString(written), file.errorString()));
    }

    // TODO enable file watcher
    qutepart_.document()->setModified(false);

    // TODO update state flags
    // TODO detect syntax if file name changed
}
