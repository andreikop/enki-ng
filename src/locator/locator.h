#pragma once

#include <memory>

#include <QAction>
#include <QDialog>
#include <QMainWindow>

#include "module.h"

#include "ui_LocatorDialog.h"

class OpenFileCommand;

class LocatorDialog: public QDialog, private Ui_LocatorDialog {
public:
    LocatorDialog(QMainWindow* parent);

private:
    std::unique_ptr<OpenFileCommand> command_;
};

class Locator: public Module {
public:
    Locator();

private:
    QAction showAction_;

private slots:
    void onTriggered();
};
