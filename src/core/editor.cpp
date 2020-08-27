#include <QApplication>
#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QMessageBox>

#include "core.h"
#include "option.h"
#include "workspace.h"

#include "editor.h"


namespace {

// Default font family, which depend on platform
QString defaultFontFamily() {
    /*
    Monaco - old Mac font,
    Menlo - modern Mac font,
    Consolas - default font for Windows Vista+
    Lucida Console - on Windows XP
    Monospace - default for other platforms
    */

    QStringList fontFamilies = {"Menlo", "Monaco", "Monospace", "Consolas", "Lucida Console"};
    QStringList availableFontFamilies = QFontDatabase().families();
    for(const auto& fontFamily: fontFamilies) {
        if (availableFontFamilies.contains(fontFamily)) {
            return fontFamily;
        }
    }

    return "Monospace";
}

Option<int> fontSizeOption("editor/font_size", -1);
Option<QString> fontFamilyOption("editor/font_family", QString::null);
Option<bool> stripTrailingWhitespaceOption("editor/strip_trailing_whitespace_on_save", true);
Option<bool> stripTrailingEmptyLinesOption("editor/strip_trailing_empty_lines_on_save", true);
Option<bool> eolAtEndOfFileOption("editor/append_eol_at_end_of_file", true);

QString fontFamily() {
    QString family = fontFamilyOption.value();

    if (family.isNull()) {
        // This value is not set as option default because QApplication
        // doesn't exist when option is initialized
        return defaultFontFamily();
    }

    return family;

}

int fontSize() {
    int size = fontSizeOption.value();

    if (size == -1) {
        // This value is not set as option default because QApplication
        // doesn't exist when option is initialized
        return QApplication::font().pointSize();
    }

    return size;
}

}  // anonymous namespace

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

    qutepart_.setFont(QFont(fontFamily(), fontSize()));

    // Enki creates own more advanced zoom actions
    qutepart_.zoomInAction()->setEnabled(false);
    qutepart_.zoomOutAction()->setEnabled(false);

}

const QString& Editor::filePath() const {
    return filePath_;
}

void Editor::setFilePath(const QString& filePath) {
    filePath_ = filePath;
}

Qutepart::Qutepart& Editor::qutepart() {
    return qutepart_;
}

void Editor::saveFile() {
    // assume file name is known
    // asume directory exists

    if (stripTrailingWhitespaceOption.value()) {
        stripTrailingWhitespace();
    }

    if (stripTrailingEmptyLinesOption.value()) {
        stripTrailingEmptyLines();
    }

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

Option<int>& Editor::getFontSizeOption() {
    return fontSizeOption;
}

QString Editor::textForSaving() const {
    if (eolAtEndOfFileOption.value()) {
        if (qutepart_.lines().last().length() != 0) {
            qutepart_.lines().append("");
        }
    }

    QStringList lines;

    for(const Qutepart::Line& line: qutepart_.lines()) {
        lines << line.text();
    }

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

void Editor::stripTrailingEmptyLines() {
    Qutepart::AtomicEditOperation op(&qutepart_);

    Qutepart::Lines lines = qutepart_.lines();

    while(lines.count() > 1 &&
          lines.last().length() == 0 &&
          lines.at(lines.count() - 2).length() == 0) {
        lines.popAt(lines.count() - 1);
    }
}
