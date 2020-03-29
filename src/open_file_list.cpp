#include <QListView>
#include <QFileInfo>
#include <QDebug>

#include "editor.h"
#include "open_file_list.h"


namespace {
    QIcon transparentIcon() {
        QPixmap pixmap(22, 22);  // 22 is copy-pasted from Enki. TODO do not hardcode
        pixmap.fill(Qt::transparent);
        return QIcon(pixmap);
    }
}

class OpenedFileModel: public QAbstractItemModel {
    /* Model which shows the list of opened files
     * in the tree view (OpenFileList)
     * It switches current file, does file sorting
     */
public:
    OpenedFileModel(QObject *parent, Workspace *workspace):
        QAbstractItemModel(parent),
        editors_(workspace->editors()),
        transparentIcon_(transparentIcon())
    {
        connect(workspace, &Workspace::editorOpened,
            this, &OpenedFileModel::onEditorOpened);
        connect(workspace, &Workspace::editorClosed,
            this, &OpenedFileModel::onEditorClosed);
        connect(workspace, &Workspace::modifiedChanged,
            this, &OpenedFileModel::onEditorModifiedChanged);

    }

    int columnCount(const QModelIndex& /*parent*/) const {
        return 1;
    }

    int rowCount(const QModelIndex& parent) const {
        if (parent.isValid())
            return 0;
        else
            return editors_.size();
    }

    bool hasChildren(const QModelIndex& parent) const {
        if (parent.isValid())
            return false;
        else
            return ! editors_.isEmpty();
    }

    QVariant data(const QModelIndex& index, int role) const {
        if ( ! index.isValid())
            return QVariant();

        Editor* editor = editors_[index.row()];

        switch (role) {
            case Qt::DecorationRole:
                if (editor->qutepart().document()->isModified()) {
                    // Using transparent icon as fallback because the list looks badly if icon theme is not set
                    return QIcon::fromTheme("document-save", transparentIcon_);
                } else {
                    return transparentIcon_;
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
            row >= editors_.size())
                return QModelIndex();

        return createIndex(row, column);
    }

    QModelIndex parent(const QModelIndex& /*index*/) const {
        return QModelIndex();
    }

    QModelIndex editorIndex(Editor* editor) const {
        return index(editors_.indexOf(editor), 0);
    }

    Editor* getEditorByIndex(const QModelIndex& index) const {
        return editors_[index.row()];
    }

private slots:
    void onEditorOpened(Editor* editor) {
        int index = editors_.indexOf(editor);
        beginInsertRows(QModelIndex(), index, index);
        endInsertRows();
    }

    void onEditorClosed(Editor* editor) {
        int index = editors_.indexOf(editor);
        beginRemoveRows(QModelIndex(), index, index);
        endRemoveRows();
    }

    void onEditorModifiedChanged(Editor* editor, bool /*modified*/) {
        int editorIndex = editors_.indexOf(editor);
        QModelIndex modelIndex = index(editorIndex, 0);
        emit dataChanged(modelIndex, modelIndex);
    }

private:
    const QList<Editor*>& editors_;
    QIcon transparentIcon_;
};

class OpenFileListView: public QListView {
public:
    OpenFileListView(QWidget* parent, Workspace* workspace):
        QListView(parent),
        workspace_(workspace),
        changingCurrent_(false)
    {
        setTextElideMode(Qt::ElideMiddle);

        model_ = new OpenedFileModel(this, workspace);
        setModel(model_);

        connect(workspace, &Workspace::currentEditorChanged,
                this, &OpenFileListView::onCurrentEditorChanged);
    }

private slots:

    void onCurrentEditorChanged(Editor* editor) {
        QModelIndex index = model_->editorIndex(editor);

        // startModifyModel()
        changingCurrent_ = true;
        setCurrentIndex(index);
        changingCurrent_ = false;

        scrollTo(index);
        // self.finishModifyModel()
    }

    void currentChanged(const QModelIndex& current, const QModelIndex& /* prev */) override {
        if (changingCurrent_) {
            return; // avoid infinite loop of signals and actions
        }

        if (current.isValid()) {
            Editor* editor = model_->getEditorByIndex(current);
            workspace_->setCurrentEditor(editor);
        }
    }

private:
    OpenedFileModel* model_;
    Workspace* workspace_;
    bool changingCurrent_;
};


OpenFileList::OpenFileList(QMainWindow *mainWindow, Workspace *workspace):
    QDockWidget("Open Files", mainWindow)
{
    setAllowedAreas(Qt::LeftDockWidgetArea);
    setFeatures(QDockWidget::DockWidgetClosable);

    OpenFileListView* treeView = new OpenFileListView(this, workspace);

    setWidget(treeView);
    mainWindow->addDockWidget(Qt::LeftDockWidgetArea, this);
}
