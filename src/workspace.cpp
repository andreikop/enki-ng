#include <algorithm>    // std::sort

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextCodec>
#include <QStatusBar>
#include <QTimer>

#include "open_file_list.h"

#include "workspace.h"


Workspace::Workspace(MainWindow* mainWindow):
    widget_(new QStackedWidget(mainWindow)),
    mainWindow_(mainWindow),
    currentEditor_(nullptr)
{
    mainWindow->setWorkspace(widget_);

    new OpenFileList(mainWindow_, this);

    connect(this, &Workspace::currentEditorChanged, mainWindow_, &MainWindow::updateTitle);
}

Workspace::~Workspace() {
    foreach(Editor* editor, editors_) {
        delete editor;
    }
}

void Workspace::openFile(const QString& filePath, int line) {
    QFileInfo fileInfo(filePath);
    QString canonicalPath = fileInfo.canonicalFilePath();

    for(auto editor: editors_) {
        if (editor->filePath() == canonicalPath) {
            setCurrentEditor(editor);
            if (line != -1) {
                editor->qutepart().goToLine(line);
            }
            return;
        }
    }

    if ( ! fileInfo.exists()) {
        showError("Failed to open file", QString("File '%0' does not exist").arg(filePath));
        return;
    }

    QString text = readFile(canonicalPath);
    if (text.isNull()) { // failed to read, error emitted
        return;
    }

    Editor *editor = new Editor(canonicalPath, text, mainWindow_);
    if (line != -1) {
        editor->qutepart().goToLine(line);
        // centerCursor() is not effective until windet is drawn. Therefore using timer
        QTimer::singleShot(0, [editor] {editor->qutepart().centerCursor();});
    }

    addEditor(editor);
    setCurrentEditor(editor);
    editor->qutepart().setFocus();
}

void Workspace::createEmptyNotSavedFile(const QString& path) {
    QFileInfo fInfo(path);
    QString absPath = path;
    if ( ! fInfo.isAbsolute()) {
        absPath = QDir::current().filePath(path);  // make absolute
    }

    Editor *editor = new Editor(absPath, QString::null, mainWindow_);

    editor->qutepart().document()->setModified(true);

    addEditor(editor);
    setCurrentEditor(editor);
    editor->qutepart().setFocus();
}

const QList<Editor*>& Workspace::editors() const {
    return editors_;
}

void Workspace::focusCurrentEditor() const {
    if (currentEditor() != nullptr) {
        currentEditor()->qutepart().setFocus();
    }
}

Editor* Workspace::currentEditor() const {
    return currentEditor_;
}

QString Workspace::readFile(const QString& filePath) {
    QFile file(filePath);

    bool ok = file.open(QIODevice::ReadOnly);
    if ( ! ok) {
        showError("Failed to open file",
            QString("Failed to open file %0: %1").arg(filePath, file.errorString()));
        return QString::null;
    }

    QByteArray data = file.readAll();
    if (data.length() != file.size()) {
        showError("Failed to open file",
            QString("Failed to read file %0: %1").arg(filePath, file.errorString()));
        return QString::null;
    }

    return QTextCodec::codecForUtfText(data, QTextCodec::codecForName("UTF-8"))->toUnicode(data);
}

void Workspace::showError(const QString& title, const QString& text) {
    QMessageBox::critical(mainWindow_, title, text, QMessageBox::Ok);
}

void Workspace::addEditor(Editor* editor) {
    editors_.append(editor);
    std::sort(editors_.begin(), editors_.end(),
        [](Editor* a, Editor* b) {return a->filePath() < b->filePath();});

    int editorIndex = editors_.indexOf(editor);
    widget_->insertWidget(editorIndex, &editor->qutepart());

    connect(editor->qutepart().document(), &QTextDocument::modificationChanged,
            mainWindow_, &QWidget::setWindowModified);

    connect(editor->qutepart().document(), &QTextDocument::modificationChanged,
            [=](bool modified) { emit modifiedChanged(editor, modified); });

    emit editorOpened(editor);
}

void Workspace::removeEditor(Editor* editor) {
    emit editorClosed(editor);
    disconnect(editor->qutepart().document(), &QTextDocument::modificationChanged,
            mainWindow_, &QWidget::setWindowModified);

    widget_->removeWidget(&editor->qutepart());
    editors_.removeOne(editor);
}

void Workspace::setCurrentEditor(Editor* editor) {
    if (editor == currentEditor_) {
        return;
    }

    if (editor != nullptr) {
        widget_->setCurrentWidget(&(editor->qutepart()));
    }

    currentEditor_ = editor;

    emit currentEditorChanged(currentEditor_);
}

void Workspace::switchFile(int offset) {
    int index = editors_.indexOf(currentEditor_);
    index += offset;
    if (index >= editors_.length()) {
        index -= editors_.length();
    } else if (index < 0) {
        index += editors_.length();
    }

    setCurrentEditor(editors_[index]);
}

void Workspace::closeCurrentFile() {
    if (currentEditor_ != nullptr) {
        Editor* editor = currentEditor_;

        if (editors_.length() > 1) {
            switchFile(-1);
        } else {
            setCurrentEditor(nullptr);
        }

        removeEditor(editor);
        delete editor;
    }
}

void Workspace::activateNextFile() {
    switchFile(+1);
}

void Workspace::activatePrevFile() {
    switchFile(-1);
}
