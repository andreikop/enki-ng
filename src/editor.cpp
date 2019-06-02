#include <QApplication>
#include <QByteArray>
#include <QFile>
#include <QMessageBox>

#include "editor.h"


Editor::Editor(const QString& filePath, const QString& text, QMainWindow* parent):
    m_filePath(filePath),
    m_qutepart(text, parent)
{
    m_qutepart.initHighlighter(m_filePath);
}

const QString& Editor::filePath() const {
    return m_filePath;
}

Qutepart::Qutepart& Editor::qutepart() {
    return m_qutepart;
}

void Editor::saveFile() {
    // assume file name is known
    // asume directory exists

    // TODO prepare text for saving. i.e. remove blank lines
    QByteArray data = m_qutepart.document()->toPlainText().toUtf8();

    // TODO disable file watcher
    QFile file(m_filePath);
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
    m_qutepart.document()->setModified(false);

    // TODO update state flags
    // TODO detect syntax if file name changed
}
