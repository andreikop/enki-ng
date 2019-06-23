#pragma once

#include <QObject>
#include <QStackedWidget>
#include <QString>


#include "qutepart.h"

#include "main_window.h"
#include "editor.h"

/* Workspace is responsible for opening and switching files
 */

class Workspace: public QObject {
    Q_OBJECT
public:
    Workspace(MainWindow& mainWindow);
    virtual ~Workspace();

    void openFile(const QString& filePath, int line=-1);

    const QList<Editor*>& editors() const;

private:
    QStackedWidget* m_widget; // owned by the main window
    QMainWindow& m_mainWindow;

    QList<Editor*> m_editors;
    Editor* m_currentEditor;

    QString readFile(const QString& filePath);
    void showError(const QString& header, const QString& text);

    void setCurrentEditor(Editor*);

private slots:
    void onFileOpen();
    void onFileSave();
    void onFileClose();
};
