#include <QFileSystemModel>
#include <QDir>
#include <QSortFilterProxyModel>
#include <QDebug>

#include "core.h"
#include "project.h"
#include "workspace.h"

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


FileTree::FileTree(QDockWidget* parent):
    QTreeView(parent)
{
    setAttribute(Qt::WA_MacShowFocusRect, false);
    setAttribute(Qt::WA_MacSmallSize);
    setContextMenuPolicy(Qt::ActionsContextMenu);
    setHeaderHidden(true);
    setUniformRowHeights(true);
    setTextElideMode(Qt::ElideMiddle);

    setModel(&core().project().filteredFsModel());
    setRootIndex(core().project().filteredFsModelRootIndex());
    connect(&core().project(), &Project::pathChanged,
        [this] {this->setRootIndex(core().project().filteredFsModelRootIndex());});

    connect(this, &FileTree::activated, this, &FileTree::onActivated);

#if 0
    self._fileActivated.connect(fileBrowser.fileActivated)

    # QDirModel loads item asynchronously, therefore we need timer for setting focus to the first item
    self._setFocusTimer = QTimer()
    self._setFocusTimer.timeout.connect(self._setFirstItemAsCurrent)
    self._setFocusTimer.setInterval(50)
    self._timerAttempts = 0
#endif
}

#if 0
void FileTree::setRootPath(const QDir& dir) {
    core().project().setRootPath(dir);
    QModelIndex newRoot = core().project().filteredFsModelRootIndex();
    setRootIndex(newRoot);

#if 0
    self._timerAttempts = 10
    self._setFocusTimer.start()
#endif
}
#endif

// File or directory doubleClicked
void FileTree::onActivated(const QModelIndex& index) const {
    QFileInfo fInfo = core().project().filteredFsModel().fileInfo(index);

    if (fInfo.isFile()) {
        // self._fileActivated.emit()
        core().workspace().openFile(fInfo.filePath());
        core().workspace().focusCurrentEditor();
    }
}
