#pragma once

#include <QTreeView>
#include <QDockWidget>


class FileTree: public QTreeView {
public:
	FileTree(QDockWidget* parent);
};