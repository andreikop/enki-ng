#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
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

    connect(mainWindow->menuBar()->fileOpenAction(), &QAction::triggered, this, &Workspace::onFileOpen);
    connect(mainWindow->menuBar()->fileSaveAction(), &QAction::triggered, this, &Workspace::onFileSave);
    connect(mainWindow->menuBar()->fileCloseAction(), &QAction::triggered, this, &Workspace::onFileClose);

    connect(mainWindow->menuBar()->viewPrevFileAction(), &QAction::triggered, this, &Workspace::onPrevFile);
    connect(mainWindow->menuBar()->viewNextFileAction(), &QAction::triggered, this, &Workspace::onNextFile);
}

Workspace::~Workspace() {
    foreach(Editor* editor, editors_) {
        delete editor;
    }
}

void Workspace::openFile(const QString& filePath, int line) {
    QFileInfo fileInfo(filePath);

    if ( ! fileInfo.exists()) {
        showError("Failed to open file", QString("File '%0' does not exist").arg(filePath));
        return;
    }

    QString canonicalPath = fileInfo.canonicalFilePath();

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
    widget_->addWidget(&editor->qutepart());

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

    updateEditMenuActions(editor);

    currentEditor_ = editor;

    emit currentEditorChanged(currentEditor_);
}

void Workspace::updateEditMenuActions(Editor* editor) {
    QMenu* editorMenu = mainWindow_->menuBar()->editorMenu();
    editorMenu->clear();
    QMenu* bookmarksMenu = mainWindow_->menuBar()->bookmarksMenu();
    bookmarksMenu->clear();

    if (editor != nullptr) {
        Qutepart::Qutepart& qpart = editor->qutepart();

        QMenu* scrollingMenu = editorMenu->addMenu("Scrolling");
        scrollingMenu->addAction(qpart.scrollUpAction());
        scrollingMenu->addAction(qpart.scrollDownAction());

        editorMenu->addAction(qpart.invokeCompletionAction());
        editorMenu->addAction(qpart.duplicateSelectionAction());

        QMenu* editLinesMenu = editorMenu->addMenu("Lines");
        editLinesMenu->addAction(qpart.moveLineUpAction());
        editLinesMenu->addAction(qpart.moveLineDownAction());

        QMenu* indentMenu = editorMenu->addMenu("Indentation");
        indentMenu->addAction(qpart.increaseIndentAction());
        indentMenu->addAction(qpart.decreaseIndentAction());

        bookmarksMenu->addAction(qpart.toggleBookmarkAction());
        bookmarksMenu->addAction(qpart.prevBookmarkAction());
        bookmarksMenu->addAction(qpart.nextBookmarkAction());
    }
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

void Workspace::onFileOpen() {
    QStringList fileNames = QFileDialog::getOpenFileNames(
        mainWindow_,
        "Open files",
        QDir::currentPath());

    foreach(const QString& fileName, fileNames) {
        openFile(fileName);
    }
}

void Workspace::onFileSave() {
    if (currentEditor_ == nullptr) {
        return;
    }

    currentEditor_->saveFile();
}

void Workspace::onFileClose() {
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

void Workspace::onNextFile() {
    switchFile(+1);
}

void Workspace::onPrevFile() {
    switchFile(-1);
}
