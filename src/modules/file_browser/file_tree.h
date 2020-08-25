#pragma once

#include <QDir>
#include <QTreeView>
#include <QDockWidget>


class FileSystemModel;
class FileBrowserFilteredModel;


class FileTree: public QTreeView {
    Q_OBJECT
public:
    FileTree(QDockWidget* parent);

private slots:
    void onActivated(const QModelIndex& index);
};
