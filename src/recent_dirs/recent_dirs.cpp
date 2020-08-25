#include <QDebug>

#include "core.h"
#include "main_window.h"
#include "option.h"
#include "project.h"

#include "recent_dirs.h"

namespace {

Option<QStringList> recentDirsOption("recent_dirs", QStringList());

const int MAX_LIST_SIZE = 3;

}

RecentDirs::RecentDirs()
{
    connect(
        core().mainWindow().menuBar()->recentDirsAction(), &QAction::triggered,
        this, &RecentDirs::onRecentDirsTriggered);
    connect(
        &core().project(), &Project::pathChanged,
        this, &RecentDirs::onDirectorySwitched);
}

void RecentDirs::onRecentDirsTriggered() {
    qDebug() << "recent dirs!" << recentDirsOption.value();
}

void RecentDirs::onDirectorySwitched(const QDir& dir) {
    QString path = dir.canonicalPath();
    QStringList recentDirValue = recentDirsOption.value();
    if (recentDirValue.contains(path)) {
        recentDirValue.removeAll(path);
    } else {
        while (recentDirValue.size() > (MAX_LIST_SIZE - 1)) {
            recentDirValue.pop_back();
        }
    }
    recentDirValue.insert(0, path);

    recentDirsOption.setValue(recentDirValue);
}
