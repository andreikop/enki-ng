#pragma once

#include <QDir>
#include <QTreeView>
#include <QDockWidget>


class FileSystemModel;
class FileBrowserFilteredModel;


class FileTree: public QTreeView {
public:
	FileTree(QDockWidget* parent);

	void setRootPath(const QDir& dir);

private:
    FileSystemModel* fsModel_;
	FileBrowserFilteredModel* filteredModel_;
};