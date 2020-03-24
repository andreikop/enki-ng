#include <QApplication>
#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QMessageBox>

#include "core.h"

#include "editor.h"


Editor::Editor(const QString& filePath, const QString& text, QMainWindow* parent):
    filePath_(filePath),
    qutepart_(parent, text),
    lineSeparator("\n")  // make configurable, autodetect
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


    // TODO prepare text for saving. i.e. remove trailing whitespace
    stripTrailingWhitespace();

    QString text = textForSaving();
    QByteArray data = text.toUtf8();

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

QString Editor::textForSaving() const {
    QStringList lines;

    for(const Qutepart::Line& line: qutepart_.lines()) {
        lines << line.text();
    }

    // TODO make configurable
    // TODO edit document but not text
    lines << "";  // to have EOL at end of file

    return lines.join(lineSeparator);
}

namespace {

int trailingSpaceCount(const QString& line) {
    for(int i = line.size() - 1; i >= 0; i--) {
        if ( ! line[i].isSpace()) {
            return line.size() - i - 1;
        }
    }
    return line.size();
}

}  // anonymous namespace

void Editor::stripTrailingWhitespace() {
    Qutepart::AtomicEditOperation op(&qutepart_);

    for(Qutepart::Line line: qutepart_.lines()) {
        int trailingCount = trailingSpaceCount(line.text());
        if (trailingCount) {
            line.remove(line.length() - trailingCount, trailingCount);
        }
    }
}
