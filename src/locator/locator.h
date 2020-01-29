#pragma once

#include <memory>

#include <QAction>
#include <QDialog>
#include <QMainWindow>

#include "module.h"

#include "ui_LocatorDialog.h"

class LocatorDialog: public QDialog, private Ui_LocatorDialog {
public:
    LocatorDialog(QMainWindow* parent);
};

class Locator: public Module {
public:
    Locator();

private:
    QAction showAction_;
    LocatorDialog widget_;
};
