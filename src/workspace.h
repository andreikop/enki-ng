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


    const QList<Editor*>& editors() const;
    Editor* currentEditor() const;

public slots:
    void focusCurrentEditor() const;

signals:
    void currentEditorChanged(Editor* editor);
    void editorOpened(Editor* editor);
    void editorClosed(Editor* editor);
    void modifiedChanged(Editor* editor, bool modified);

private:
    QStackedWidget* m_widget; // owned by the main window
    MainWindow* mainWindow_;

    QList<Editor*> m_editors;
    Editor* m_currentEditor;

    QString readFile(const QString& filePath);
    void showError(const QString& header, const QString& text);

    void addEditor(Editor*);
    void removeEditor(Editor*);
    void setCurrentEditor(Editor*);

    void switchFile(int offset);

    void setActionsInEditorMenu(Editor* editor);

private slots:
    void onFileOpen();
    void onFileSave();
    void onFileClose();

    void onNextFile();
    void onPrevFile();
};
