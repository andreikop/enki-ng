#pragma once

#include <memory>

#include <QObject>
#include <QDir>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>

/* This class provides API to work with current project.

Project for Enki is a directory with files
Only one project can be opened and it is current directory of the process
*/

class FilteredFsModel: public QSortFilterProxyModel {
public:
    QFileInfo fileInfo(const QModelIndex& index) const;

    // TODO I'm not sure this model should contain this method.
    // FileTree needs only one column, but probably it should implement own model
    int columnCount(const QModelIndex& parent=QModelIndex()) const override;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;
};


class Project: public QObject {
    Q_OBJECT
public:
    Project();
    void setPath(const QDir& path);
    QDir path() const;

    /* Get a list of the project files.
    File system is scanned asyncronously,
    the list might be bigger if you call this method later.
     */
    QStringList fileList();

    /* Filtered FS model which contains files not excluded by file name filters

    This model contains FS cache and can be used by multiple Enki modules.
    It is more effective than have own model in every module which needs FS tree.

    NOTE the model contains full FS tree, not only the project.
    Use filteredFsModelRootIndex() to get project root.
     */
    FilteredFsModel& filteredFsModel();
    QModelIndex filteredFsModelRootIndex() const;

signals:
    void pathChanged(const QDir& path) const;
    void fileListUpdated() const;

private:
    QDir path_;
    QFileSystemModel fsModel_;
    FilteredFsModel filteredFsModel_;

    std::unique_ptr<QStringList> fileListCache_;

    void findFilesRecursively(const QModelIndex& filteredModelIndex, QStringList& result);

    void startChildNodeLoading(const QString& directory);

private slots:
    void onDirectoryLoaded(const QString& directory);
};
