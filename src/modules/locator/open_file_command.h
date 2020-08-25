#pragma once

#include <QObject>

#include "lib/locator/locator_list_model.h"


class OpenFileCommand: public QObject {
    Q_OBJECT

public:
    OpenFileCommand();
    QAbstractItemModel& model();

public slots:
    void setCommandText(const QString& text);
    void onItemActivated(const QModelIndex& index);

signals:
    void done();

private:
    LocatorListModel model_;
};
