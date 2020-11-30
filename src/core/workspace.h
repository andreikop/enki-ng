#pragma once

#include <QObject>
#include <QStackedWidget>
#include <QString>


#include "qutepart/qutepart.h"

#include "main_window.h"
#include "editor.h"

/* Workspace is responsible for opening and switching files
 */

class Workspace: public QObject {
    Q_OBJECT
public:
    Workspace(MainWindow* mainWindow);
    virtual ~Workspace();

    void openFile(const QString& filePath, int line=-1);
    Editor* createNewFile();
    Editor* createEmptyNotSavedFile(const QString& path);


    const QList<Editor*>& editors() const;
    Editor* currentEditor() const;
    void setCurrentEditor(Editor*);

public slots:
    void focusCurrentEditor() const;

signals:
    void currentEditorChanged(Editor* editor);
    void editorOpened(Editor* editor);
    void editorClosed(Editor* editor);
    void modifiedChanged(Editor* editor, bool modified);
    void editorFilePathChanged(Editor* editor, const QString& newPath);

private:
    QStackedWidget* widget_; // owned by the main window
    MainWindow* mainWindow_;

    QList<Editor*> editors_;
    Editor* currentEditor_;

    QString readFile(const QString& filePath);
    void showError(const QString& header, const QString& text);

    void addEditor(Editor*);
    void removeEditor(Editor*);

    void switchFile(int offset);

public slots:
    void closeCurrentFile();
    void activateNextFile();
    void activatePrevFile();
};
