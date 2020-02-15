#include <QDebug>

#include "project.h"


QFileInfo FilteredFsModel::fileInfo(const QModelIndex& index) const {
    QModelIndex sourceIndex = mapToSource(index);
    QString path = sourceModel()->data(sourceIndex, QFileSystemModel::FilePathRole).toString();
    return QFileInfo(path);
}

int FilteredFsModel::columnCount(const QModelIndex& /*parent*/) const {
    return 1;
}

void FilteredFsModel::setIgnoredFilePatterns(const QList<QRegExp>& patterns) {
    ignoredFileWildcards_ = patterns;
}

void FilteredFsModel::setIgnoredDirectoryPatterns(const QList<QRegExp>& patterns) {
    ignoredDirectoryWildcards_ = patterns;
}

void FilteredFsModel::setCanonicalRootPath(const QString& path) {
    canonicalRootPath_ = path;
}

bool FilteredFsModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    QModelIndex sourceIndex = sourceModel()->index(sourceRow, 0, sourceParent);
    QString path = sourceModel()->data(sourceIndex, QFileSystemModel::FilePathRole).toString();
    QFileInfo fInfo = QFileInfo(path);
    if (fInfo.isDir()) {
        // It is important not to ignore parent directories.
        if (canonicalRootPath_.startsWith(fInfo.canonicalFilePath())) {
            return true;  // always allow parents (higher than project root)
        }

        return ! wildcardListMatches(ignoredDirectoryWildcards_, fInfo);
    } else {
        return ! wildcardListMatches(ignoredFileWildcards_, fInfo);
    }
}

bool FilteredFsModel::wildcardListMatches(const QList<QRegExp>& wildcards, const QFileInfo& fInfo) {
    QString path = fInfo.fileName();

    foreach(const QRegExp& rx, wildcards) {
        if (rx.exactMatch(path)) {
            return true;
        }
    }

    return false;
}

Project::Project():
    path_(QDir::current())
{
    // setRootPath for the model wasn't called to avoid monitoring home dir
    fsModel_.setNameFilterDisables(false);
    fsModel_.setFilter(QDir::AllDirs | QDir::AllEntries | QDir::CaseSensitive | QDir::NoDotAndDotDot);

    // create proxy model
    filteredFsModel_.setCanonicalRootPath(path_.canonicalPath());
    filteredFsModel_.setSourceModel(&fsModel_);

    // TODO hardcoded patterns. Replace with something better
    QList<QRegExp> ignoredFilePatterns;
    for (QString pattern : {"*.o", "*.a", "*.so"})  {
        ignoredFilePatterns << QRegExp(pattern, Qt::CaseSensitive, QRegExp::WildcardUnix);
    }
    filteredFsModel_.setIgnoredFilePatterns(ignoredFilePatterns);

    QList<QRegExp> ignoredDirectoryPatterns;
    ignoredDirectoryPatterns << QRegExp("build", Qt::CaseSensitive, QRegExp::WildcardUnix);
    filteredFsModel_.setIgnoredDirectoryPatterns(ignoredDirectoryPatterns);

    connect(&fsModel_, &QFileSystemModel::directoryLoaded, this, &Project::onDirectoryLoaded);

    setPath(QDir::current());
}

void Project::setPath(const QDir& path) {
    path_ = path;
    fsModel_.setRootPath(path.path());
    filteredFsModel_.setCanonicalRootPath(path.canonicalPath());
    fileListCache_.reset();
    emit(pathChanged(path));
}

QDir Project::path() const {
    return path_;
}

QStringList Project::fileList() {
    if (! fileListCache_) {
        fileListCache_ = std::make_unique<QStringList>();
        findFilesRecursively(filteredFsModelRootIndex(), *fileListCache_);
    }

    return *fileListCache_;
}

FilteredFsModel& Project::filteredFsModel() {
    return filteredFsModel_;
}

QModelIndex Project::filteredFsModelRootIndex() const {
    QModelIndex fsModelIndex = fsModel_.index(fsModel_.rootPath());

    return filteredFsModel_.mapFromSource(fsModelIndex);
}

void Project::findFilesRecursively(const QModelIndex& filteredModelIndex, QStringList& result) {
    QFileInfo fInfo = filteredFsModel_.fileInfo(filteredModelIndex);
    if (fInfo.isFile()) {
        result << path_.relativeFilePath(fInfo.filePath());
    } else if (fInfo.isDir()) {
        for(int childIdx = 0; childIdx < filteredFsModel_.rowCount(filteredModelIndex); childIdx++) {
            findFilesRecursively(filteredFsModel_.index(childIdx, 0, filteredModelIndex), result);
        }
    }
}

void Project::startChildNodeLoading(const QString& directory) {
    QModelIndex filteredModelIndex = filteredFsModel_.mapFromSource(fsModel_.index(directory));

    int childCount = filteredFsModel_.rowCount(filteredModelIndex);
    for(int childIdx = 0; childIdx < childCount; childIdx++) {
        filteredFsModel_.fetchMore(filteredFsModel_.index(childIdx, 0, filteredModelIndex));
    }
}

void Project::onDirectoryLoaded(const QString& directory) {
    fileListCache_.reset();

    startChildNodeLoading(directory);

    // TODO delay signal until finished loading children?
    emit(fileListUpdated());
}
