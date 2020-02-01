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
private:
    LocatorModel model_;
};
