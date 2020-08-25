#include <QAction>
#include <QDebug>

#include "core.h"
#include "option.h"
#include "main_window.h"
#include "subprocess.h"

#include "project.h"

namespace {

const int LOADED_DIRECTORIES_LIMIT = 256;

Option<QStringList> ignoredFilePatterns(
        "projects/ignored_file_patterns",
        {"*.o", "*.a", "*.so", "*.pyc"});

Option<QStringList> ignoredDirectoryPatterns(
        "projects/ignored_directory_patterns",
        {".git", ".svn", "__pycache__"});

}  // anonymous namespace

Project::Project():
    path_(QDir::current()),
    countOfLoadedDirectories_(0)
{
    // setRootPath for the model wasn't called to avoid monitoring home dir
    fsModel_.setNameFilterDisables(false);
    fsModel_.setFilter(QDir::AllDirs | QDir::AllEntries | QDir::CaseSensitive | QDir::NoDotAndDotDot);

    // create proxy model
    filteredFsModel_.setRootPath(path_.canonicalPath());
    filteredFsModel_.setSourceModel(&fsModel_);
    filteredFsModel_.setIgnoredFilePatterns(ignoredFilePatterns.value());
    filteredFsModel_.setIgnoredDirectoryPatterns(ignoredDirectoryPatterns.value());

    connect(&fsModel_, &QFileSystemModel::directoryLoaded, this, &Project::onDirectoryLoaded);

    setPath(QDir::current());
}

void Project::setPath(const QDir& path) {
    QDir absPath = QDir(path.absolutePath());

    path_ = absPath;

    QDir::setCurrent(absPath.path());


    fsModel_.setRootPath(absPath.path());
    filteredFsModel_.setRootPath(absPath.canonicalPath());

    // FIXME blocking call!!!!
    SubProcessResult gitRes = runSubProcess(
            "git", {"ls-files", "--others", "--ignored", "--exclude-standard", "--directory"});
    if (gitRes.exitCode == 0) {
        // TODO use cross platform EOL?
        QStringList fileList = gitRes.stdOut.split("\n", QString::SkipEmptyParts);
        filteredFsModel_.setGitIgnoredFileList(fileList);
    } else {
        filteredFsModel_.setGitIgnoredFileList({});
    }

    fileListCache_.reset();
    countOfLoadedDirectories_ = 0;
    emit(pathChanged(absPath));
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

void Project::onDirectoryLoaded(const QString& directory ) {
    if (countOfLoadedDirectories_ > LOADED_DIRECTORIES_LIMIT) {
        return;
    }

    fileListCache_.reset();

    startChildNodeLoading(directory);
    countOfLoadedDirectories_++;

    // TODO delay signal until finished loading children?
    emit(fileListUpdated());
}
