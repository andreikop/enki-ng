#include <QTreeView>
#include <QFileInfo>

#include "editor.h"
#include "file_explorer.h"


class OpenedFileModel: public QAbstractItemModel {
    /* Model which shows the list of opened files
     * in the tree view (FileExplorer)
     * It switches current file, does file sorting
     */
public:
    OpenedFileModel(QObject *parent, Workspace *workspace):
        m_editors(workspace->editors())
    {}

    int columnCount(const QModelIndex& parent) const {
        return 1;
    }

    int rowCount(const QModelIndex& parent) const {
        if (parent.isValid())
            return 0;
        else
            return m_editors.size();
    }

    bool hasChildren(const QModelIndex& parent) const {
        if (parent.isValid())
            return false;
        else
            return ! m_editors.isEmpty();
    }

    QVariant data(const QModelIndex& index, int role) const {
        if ( ! index.isValid())
            return QVariant();

        Editor* editor = m_editors[index.row()];

        switch (role) {
            case Qt::DecorationRole:
                return QVariant();
            case Qt::DisplayRole:
                return QFileInfo(editor->filePath()).fileName();
            case Qt::EditRole:
            case Qt::ToolTipRole:
                return editor->filePath();
            default:
                return QVariant();
        }
    }

    QModelIndex index(int row, int column, const QModelIndex& parent=QModelIndex()) const {
        if (parent.isValid() ||
            column > 0 ||
            column < 0 ||
            row < 0 ||
            row >= m_editors.size())
                return QModelIndex();

        return createIndex(row, column);
    }

    QModelIndex parent(const QModelIndex& index) const {
        return QModelIndex();
    }

private:
    const QList<Editor*>& m_editors;
};


FileExplorer::FileExplorer(QMainWindow *mainWindow, Workspace *workspace):
    QDockWidget("Opened files", mainWindow)
{
    setAllowedAreas(Qt::LeftDockWidgetArea);
    setFeatures(QDockWidget::DockWidgetClosable);


    QTreeView* treeView = new QTreeView(this);
    treeView->setHeaderHidden(true);
    treeView->setRootIsDecorated(false);
    treeView->setTextElideMode(Qt::ElideMiddle);
    treeView->setUniformRowHeights(true);
    treeView->setModel(new OpenedFileModel(treeView, workspace));

    setWidget(treeView);
    mainWindow->addDockWidget(Qt::LeftDockWidgetArea, this);
}
