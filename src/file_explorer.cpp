#include <QTreeView>
#include <QFileInfo>
#include <QDebug>

#include "editor.h"
#include "file_explorer.h"


namespace {
    QIcon transparentIcon() {
        QPixmap pixmap(22, 22);  // 22 is copy-pasted from Enki. TODO do not hardcode
        pixmap.fill(Qt::transparent);
        return QIcon(pixmap);
    }
}

class OpenedFileModel: public QAbstractItemModel {
    /* Model which shows the list of opened files
     * in the tree view (FileExplorer)
     * It switches current file, does file sorting
     */
public:
    OpenedFileModel(QObject *parent, Workspace *workspace):
        m_editors(workspace->editors()),
        m_notModifiedIcon(transparentIcon())
    {
        connect(workspace, &Workspace::editorOpened,
            this, &OpenedFileModel::onEditorOpened);
        connect(workspace, &Workspace::editorClosed,
            this, &OpenedFileModel::onEditorClosed);
        connect(workspace, &Workspace::modifiedChanged,
            this, &OpenedFileModel::onEditorModifiedChanged);

    }

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
                if (editor->qutepart().document()->isModified()) {
                    return QIcon::fromTheme("document-save");
                } else {
                    return m_notModifiedIcon;
                }
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

    QModelIndex editorIndex(Editor* editor) const {
        return index(m_editors.indexOf(editor), 0);
    }

private slots:
    void onEditorOpened(Editor* editor) {
        int index = m_editors.indexOf(editor);
        beginInsertRows(QModelIndex(), index, index);
        endInsertRows();
    }

    void onEditorClosed(Editor* editor) {
        int index = m_editors.indexOf(editor);
        beginRemoveRows(QModelIndex(), index, index);
        endRemoveRows();
    }

    void onEditorModifiedChanged(Editor* editor, bool modified) {
        int editorIndex = m_editors.indexOf(editor);
        QModelIndex modelIndex = index(editorIndex, 0);
        emit dataChanged(modelIndex, modelIndex);
    }

private:
    const QList<Editor*>& m_editors;
    QIcon m_notModifiedIcon;
};

class FileExplorerTreeView: public QTreeView {
public:
    FileExplorerTreeView(QObject* parent, Workspace* workspace) {
        setHeaderHidden(true);
        setRootIsDecorated(false);
        setTextElideMode(Qt::ElideMiddle);
        setUniformRowHeights(true);

        m_model = new OpenedFileModel(this, workspace);
        setModel(m_model);

        connect(workspace, &Workspace::currentEditorChanged,
                this, &FileExplorerTreeView::onCurrentEditorChanged);
    }

private slots:

    void onCurrentEditorChanged(Editor* editor) {
        QModelIndex index = m_model->editorIndex(editor);

        // startModifyModel()
        setCurrentIndex(index);
        scrollTo(index);
        // self.finishModifyModel()
    }

private:
    OpenedFileModel* m_model;
};


FileExplorer::FileExplorer(QMainWindow *mainWindow, Workspace *workspace):
    QDockWidget("Opened files", mainWindow)
{
    setAllowedAreas(Qt::LeftDockWidgetArea);
    setFeatures(QDockWidget::DockWidgetClosable);

    FileExplorerTreeView* treeView = new FileExplorerTreeView(this, workspace);

    setWidget(treeView);
    mainWindow->addDockWidget(Qt::LeftDockWidgetArea, this);
}
