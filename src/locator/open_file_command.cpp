#include "core.h"

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
