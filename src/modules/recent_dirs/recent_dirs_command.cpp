#include "core/core.h"
#include "core/project.h"

#include "recent_dirs_command.h"


RecentDirsCommand::RecentDirsCommand(const QStringList& dirs):
    model_(LocatorListModel(dirs))
{}


QAbstractItemModel& RecentDirsCommand::model() {
    return model_;
}

void RecentDirsCommand::setCommandText(const QString& text) {
    model_.setCommandText(text);
}

void RecentDirsCommand::onItemActivated(const QModelIndex& index) {
    const QString& path = model_.text(index);
    core().project().setPath(QDir(path));
    emit(done());
}
