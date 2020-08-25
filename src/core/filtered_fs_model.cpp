#include <QFileSystemModel>

#include "filtered_fs_model.h"


QFileInfo FilteredFsModel::fileInfo(const QModelIndex& index) const {
    QModelIndex sourceIndex = mapToSource(index);
    QString path = sourceModel()->data(sourceIndex, QFileSystemModel::FilePathRole).toString();
    return QFileInfo(path);
}

int FilteredFsModel::columnCount(const QModelIndex& /*parent*/) const {
    return 1;
}

void FilteredFsModel::setIgnoredFilePatterns(const QStringList& patterns) {
    ignoredFileWildcards_ = fromStringList(patterns);
}

void FilteredFsModel::setIgnoredDirectoryPatterns(const QStringList& patterns) {
    ignoredDirectoryWildcards_ = fromStringList(patterns);
}

void FilteredFsModel::setGitIgnoredFileList(const QStringList& fileList) {
    gitIgnoredDirs_.clear();
    gitIgnoredFiles_.clear();

    for(const QString& filePath: fileList) {
        if (filePath.endsWith("/")) {  // TODO on Windows?
            gitIgnoredDirs_ << filePath.left(filePath.length() - 1);  // add w/o /
        } else {
            gitIgnoredFiles_ << filePath;
        }
    }
}

void FilteredFsModel::setRootPath(const QString& path) {
    rootDir_ = QDir(path);
}

bool FilteredFsModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    QModelIndex sourceIndex = sourceModel()->index(sourceRow, 0, sourceParent);
    QString path = sourceModel()->data(sourceIndex, QFileSystemModel::FilePathRole).toString();
    QFileInfo fInfo = QFileInfo(path);

    if (fInfo.isDir()) {
        // It is important not to ignore parent directories.
        if (rootDir_.canonicalPath().startsWith(fInfo.canonicalFilePath())) {
            return true;  // always allow parents (higher than project root)
        }

        if (wildcardListMatches(ignoredDirectoryWildcards_, fInfo)) {
            // Ignored by Enki settings
            return false;
        }

        QString relativePath = rootDir_.relativeFilePath(path);
        if (gitIgnoredDirs_.contains(relativePath)) {
            // Ignored by Git settings
            return false;
        }

        return true;
    } else {
        if (wildcardListMatches(ignoredFileWildcards_, fInfo)) {
            // Ignored by Enki settings
            return false;
        }

        QString relativePath = rootDir_.relativeFilePath(path);
        if (gitIgnoredFiles_.contains(relativePath)) {
            // Ignored by Git settings
            return false;
        }

        return true;
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

QList<QRegExp> FilteredFsModel::fromStringList(const QStringList& wildcards) {
    QList<QRegExp> result;

    foreach(const QString& pattern, wildcards) {
        result << QRegExp(pattern, Qt::CaseSensitive, QRegExp::WildcardUnix);
    }

    return result;
}
