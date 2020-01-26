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

	void setRootPath(const QDir& dir);

private:
    FileSystemModel* fsModel_;
	FileBrowserFilteredModel* filteredModel_;

private slots:
	void onActivated(const QModelIndex& index) const;
};