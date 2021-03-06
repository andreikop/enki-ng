#include <QDebug>

#include "core/core.h"
#include "core/main_window.h"
#include "core/option.h"
#include "core/project.h"

#include "lib/locator/locator_dialog.h"

#include "recent_dirs.h"
#include "recent_dirs_command.h"

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
    LocatorDialog LocatorDialog(
        &core().mainWindow(),
        std::make_shared<RecentDirsCommand>(
            recentDirsOption.value()));
    LocatorDialog.exec();
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
