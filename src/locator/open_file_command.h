#pragma once

#include <QObject>

#include "locator_model.h"


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
    LocatorModel model_;
};
