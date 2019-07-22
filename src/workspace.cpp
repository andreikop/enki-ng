#include <QFile>
#include <QFileDialog>
#include <QTextCodec>
#include <QFileInfo>
#include <QStatusBar>
#include <QMessageBox>
#include <QDebug>

#include "file_explorer.h"

#include "workspace.h"


Workspace::Workspace(MainWindow& mainWindow):
    m_widget(new QStackedWidget(&mainWindow)),
    m_mainWindow(mainWindow),
    m_currentEditor(nullptr)
{
    m_mainWindow.setCentralWidget(m_widget);

    new FileExplorer(&m_mainWindow, this);

    connect(mainWindow.menuBar()->fileOpenAction, &QAction::triggered, this, &Workspace::onFileOpen);
    connect(mainWindow.menuBar()->fileSaveAction, &QAction::triggered, this, &Workspace::onFileSave);
    connect(mainWindow.menuBar()->fileCloseAction, &QAction::triggered, this, &Workspace::onFileClose);

    connect(mainWindow.menuBar()->viewPrevFileAction, &QAction::triggered, this, &Workspace::onPrevFile);
    connect(mainWindow.menuBar()->viewNextFileAction, &QAction::triggered, this, &Workspace::onNextFile);
}

Workspace::~Workspace() {
    foreach(Editor* editor, m_editors) {
        delete editor;
    }
}

void Workspace::openFile(const QString& filePath, int /*line*/) {
    QFileInfo fileInfo(filePath);

    if ( ! fileInfo.exists()) {
        showError("Failed to open file", QString("File '%0' does not exist").arg(filePath));
        qDebug() << filePath;
        return;
    }

    QString canonicalPath = fileInfo.canonicalFilePath();

    QString text = readFile(canonicalPath);
    if (text.isNull()) { // failed to read, error emitted
        return;
    }

    Editor *editor = new Editor(canonicalPath, text, &m_mainWindow);

    addEditor(editor);
    setCurrentEditor(editor);
    editor->qutepart().setFocus();
}

const QList<Editor*>& Workspace::editors() const {
    return m_editors;
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
    if (data.isEmpty() && ( ! file.errorString().isEmpty())) {
        showError("Failed to open file",
            QString("Failed to read file %0: %1").arg(filePath, file.errorString()));
        return QString::null;
    }

    return QTextCodec::codecForUtfText(data, QTextCodec::codecForName("UTF-8"))->toUnicode(data);
}

void Workspace::showError(const QString& title, const QString& text) {
    QMessageBox::critical(&m_mainWindow, title, text, QMessageBox::Ok);
}

void Workspace::addEditor(Editor* editor) {
    m_editors.append(editor);
    m_widget->addWidget(&editor->qutepart());

    connect(editor->qutepart().document(), &QTextDocument::modificationChanged,
            &m_mainWindow, &QWidget::setWindowModified);

    connect(editor->qutepart().document(), &QTextDocument::modificationChanged,
            [=](bool modified) { emit modifiedChanged(editor, modified); });

    emit editorOpened(editor);
}

void Workspace::removeEditor(Editor* editor) {
    emit editorClosed(editor);
    disconnect(editor->qutepart().document(), &QTextDocument::modificationChanged,
            &m_mainWindow, &QWidget::setWindowModified);

    m_widget->removeWidget(&editor->qutepart());
    m_editors.removeOne(editor);
}

void Workspace::setCurrentEditor(Editor* editor) {
    if (editor == m_currentEditor) {
        return;
    }

    m_mainWindow.setWindowTitle(QString("%1[*]").arg(QFileInfo(editor->filePath()).fileName()));
    m_mainWindow.setWindowModified(editor->qutepart().document()->isModified());

    m_widget->setCurrentWidget(&(editor->qutepart()));

    m_currentEditor = editor;

    emit currentEditorChanged(m_currentEditor);
}

void Workspace::switchFile(int offset) {
    int index = m_editors.indexOf(m_currentEditor);
    index += offset;
    if (index >= m_editors.length()) {
        index -= m_editors.length();
    } else if (index < 0) {
        index += m_editors.length();
    }

    setCurrentEditor(m_editors[index]);
}

void Workspace::onFileOpen() {
    QStringList fileNames = QFileDialog::getOpenFileNames(
        &m_mainWindow,
        "Open files",
        QDir::currentPath());

    foreach(const QString& fileName, fileNames) {
        openFile(fileName);
    }
}

void Workspace::onFileSave() {
    if (m_currentEditor == nullptr) {
        return;
    }

    m_currentEditor->saveFile();
}

void Workspace::onFileClose() {
    if (m_currentEditor != nullptr) {
        Editor* editor = m_currentEditor;
        switchFile(-1);
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
