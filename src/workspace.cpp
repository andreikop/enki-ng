#include <QFile>
#include <QFileDialog>
#include <QTextCodec>
#include <QFileInfo>
#include <QStatusBar>
#include <QMessageBox>
#include <QDebug>

#include "workspace.h"


Workspace::Workspace(MainWindow& mainWindow):
    m_mainWindow(mainWindow),
    m_currentEditor(nullptr)
{
    connect(mainWindow.menuBar()->fileOpenAction, &QAction::triggered, this, &Workspace::onFileOpen);
    connect(mainWindow.menuBar()->fileSaveAction, &QAction::triggered, this, &Workspace::onFileSave);
    connect(mainWindow.menuBar()->fileCloseAction, &QAction::triggered, this, &Workspace::onFileClose);
}

Workspace::~Workspace() {
    foreach(Editor* editor, m_files) {
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

    m_files.append(editor);

    setCurrentEditor(editor);
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

void Workspace::setCurrentEditor(Editor* editor) {
    if (m_currentEditor != nullptr) {
        onFileClose(); // TODO now only one file is supported
    }

    m_mainWindow.setWindowTitle(QString("%1[*]").arg(QFileInfo(editor->filePath()).fileName()));
    connect(editor->qutepart().document(), &QTextDocument::modificationChanged,
            &m_mainWindow, &QWidget::setWindowModified);

    m_mainWindow.setCentralWidget(&editor->qutepart());
    m_currentEditor = editor;
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
        m_files.removeOne(m_currentEditor);
        delete m_currentEditor;
        m_currentEditor = nullptr;
    }
}
