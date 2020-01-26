#include <QFileSystemModel>
#include <QDir>
#include <QSortFilterProxyModel>
#include <QDebug>

#include "file_tree.h"

//Extended QFileSystemModel.
//Shows full path on tool tips
class FileSystemModel: public QFileSystemModel {
	using QFileSystemModel::QFileSystemModel;

    QVariant data(const QModelIndex& index, int role) {
        if (role == Qt::ToolTipRole) {
            return filePath(index);
        } else {
            return QFileSystemModel::data(index, role);
        }
    }
};

/* Model filters out files using negative filter.
i.e. does not show .o .pyc and other temporary files
*/
class FileBrowserFilteredModel: public QSortFilterProxyModel {
	using QSortFilterProxyModel::QSortFilterProxyModel;

    //Column count for the model
    int columnCount(const QModelIndex& parent=QModelIndex()) const override {
        return 1;
    }

    // Check if node has children. QAbstractItemModel standard method
    bool hasChildren(const QModelIndex& parent=QModelIndex()) const override {
        return sourceModel()->hasChildren(mapToSource(parent));
    }

    // Main method. Check if file matches filter
    // TODO implement
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override {
#if 0
        if sourceParent == QModelIndex():
            return True
        return not core.fileFilter().regExp().match(sourceParent.child(sourceRow, 0).data())
#else
    return true;
#endif
    }
};

FileTree::FileTree(QDockWidget* parent):
    QTreeView(parent),
    fsModel_(new FileSystemModel(this)),
    filteredModel_(new FileBrowserFilteredModel(this))
{
    setAttribute(Qt::WA_MacShowFocusRect, false);
    setAttribute(Qt::WA_MacSmallSize);
    setContextMenuPolicy(Qt::ActionsContextMenu);
    setHeaderHidden(true);
    setUniformRowHeights(true);
    setTextElideMode(Qt::ElideMiddle);

    // dir model
    fsModel_->setNameFilterDisables(false);
    fsModel_->setFilter(QDir::AllDirs | QDir::AllEntries | QDir::CaseSensitive | QDir::NoDotAndDotDot);
    // self._dirsModel.directoryLoaded.connect(self.setFocus)  TODO don't have this signal in my Qt version

    // create proxy model
    filteredModel_->setSourceModel(fsModel_);

    setModel(filteredModel_);
    setRootPath(QDir::current());

#if 0
    connect(this, &FileTree::activated, this, &FileTree::onActivated);

    self._fileActivated.connect(fileBrowser.fileActivated)

    # QDirModel loads item asynchronously, therefore we need timer for setting focus to the first item
    self._setFocusTimer = QTimer()
    self._setFocusTimer.timeout.connect(self._setFirstItemAsCurrent)
    self._setFocusTimer.setInterval(50)
    self._timerAttempts = 0
#endif
}

void FileTree::setRootPath(const QDir& dir) {
    QModelIndex index = fsModel_->setRootPath(dir.path());

    // set current path
    filteredModel_->invalidate();
    QModelIndex newRoot = filteredModel_->mapFromSource(index);
    setRootIndex(newRoot);

#if 0
    self._timerAttempts = 10
    self._setFocusTimer.start()
#endif
}