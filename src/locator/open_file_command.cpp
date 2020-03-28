#include <QDebug>

#include "core.h"
#include "workspace.h"

#include "open_file_command.h"


OpenFileCommand::OpenFileCommand():
    model_(LocatorModel(*this))
{}

QAbstractItemModel& OpenFileCommand::model() {
    return model_;
}

void OpenFileCommand::setCommandText(const QString& text) {
    model_.setCommandText(text);
}

void OpenFileCommand::onItemActivated(const QModelIndex& index) {
    const QString& path = model_.filePath(index);
    core().workspace().openFile(path);
    emit(done());
}
