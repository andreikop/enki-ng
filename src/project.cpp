#include "project.h"


QFileInfo FilteredFsModel::fileInfo(const QModelIndex& index) const {
    QModelIndex sourceIndex = mapToSource(index);
    QString path = sourceModel()->data(sourceIndex, QFileSystemModel::FilePathRole).toString();
    return QFileInfo(path);
}

int FilteredFsModel::columnCount(const QModelIndex& parent) const {
    return 1;
}

bool FilteredFsModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const {
#if 0
        if sourceParent == QModelIndex():
            return True
        return not core.fileFilter().regExp().match(sourceParent.child(sourceRow, 0).data())
#else
    return true;
#endif
}

Project::Project():
    path_(QDir::current())
{
    // setRootPath for the model wasn't called to avoid monitoring home dir
    fsModel_.setNameFilterDisables(false);
    fsModel_.setFilter(QDir::AllDirs | QDir::AllEntries | QDir::CaseSensitive | QDir::NoDotAndDotDot);

    // create proxy model
    filteredFsModel_.setSourceModel(&fsModel_);

    setPath(QDir::current());
}

void Project::setPath(const QDir& path) {
    path_ = path;
    fsModel_.setRootPath(path.path());
    fileListCache_ = QStringList();
    emit(pathChanged(path));
}

QDir Project::path() const {
    return path_;
}

QStringList Project::fileList() const {
    // FIXME implement
    return QStringList();
}

FilteredFsModel& Project::filteredFsModel() {
    return filteredFsModel_;
}

QModelIndex Project::filteredFsModelRootIndex() const {
    QModelIndex fsModelIndex = fsModel_.index(fsModel_.rootPath());

    return filteredFsModel_.mapFromSource(fsModelIndex);
}
