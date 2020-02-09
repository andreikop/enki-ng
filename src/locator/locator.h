#pragma once

#include <QAction>

#include "module.h"


class Locator: public Module {
public:
    Locator();

private:
    QAction showAction_;

private slots:
    void onTriggered();
};
