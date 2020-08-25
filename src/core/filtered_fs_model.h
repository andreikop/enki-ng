#pragma once

#include <QDir>
#include <QFileInfo>
#include <QSortFilterProxyModel>


class FilteredFsModel: public QSortFilterProxyModel {
public:
    QFileInfo fileInfo(const QModelIndex& index) const;

    // TODO I'm not sure this model should contain this method.
    // FileTree needs only one column, but probably it should implement own model
    int columnCount(const QModelIndex& parent=QModelIndex()) const override;

    void setIgnoredFilePatterns(const QStringList& patterns);
    void setIgnoredDirectoryPatterns(const QStringList& patterns);
    void setGitIgnoredFileList(const QStringList& fileList);
    void setRootPath(const QString& path);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
    static bool wildcardListMatches(const QList<QRegExp>& wildcards, const QFileInfo& fInfo);
    static QList<QRegExp> fromStringList(const QStringList& wildcards);

    QList<QRegExp> ignoredFileWildcards_;
    QList<QRegExp> ignoredDirectoryWildcards_;

    QStringList gitIgnoredDirs_;
    QStringList gitIgnoredFiles_;

    QDir rootDir_;
};
