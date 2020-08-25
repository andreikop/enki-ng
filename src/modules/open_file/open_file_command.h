#pragma once

#include "lib/locator/locator_command.h"
#include "lib/locator/locator_list_model.h"


class OpenFileCommand: public LocatorCommand {
    Q_OBJECT

public:
    OpenFileCommand();
    QAbstractItemModel& model() override;

public slots:
    void setCommandText(const QString& text) override;
    void onItemActivated(const QModelIndex& index) override;

private:
    LocatorListModel model_;
};
