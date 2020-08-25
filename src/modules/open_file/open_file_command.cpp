#include <QDebug>

#include "core/core.h"
#include "core/workspace.h"
#include "core/project.h"

#include "open_file_command.h"


OpenFileCommand::OpenFileCommand():
    model_(LocatorListModel(core().project().fileList()))
{}

QAbstractItemModel& OpenFileCommand::model() {
    return model_;
}

void OpenFileCommand::setCommandText(const QString& text) {
    model_.setCommandText(text);
}

void OpenFileCommand::onItemActivated(const QModelIndex& index) {
    const QString& path = model_.text(index);
    core().workspace().openFile(path);
    emit(done());
}
