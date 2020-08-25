#pragma once

#include <QAction>

#include "core/module.h"


class OpenFile: public Module {
    Q_OBJECT
public:
    OpenFile();

private:
    QAction showAction_;

private slots:
    void onTriggered();
};
