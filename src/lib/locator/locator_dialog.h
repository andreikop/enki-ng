#pragma once

#include <memory>

#include <QDialog>
#include <QMainWindow>

#include "lib/locator/locator_command.h"

#include "ui_LocatorDialog.h"


class LocatorDialog: public QDialog, private Ui_LocatorDialog {
public:
    LocatorDialog(QMainWindow* parent, std::shared_ptr<LocatorCommand> command);

private:
    std::shared_ptr<LocatorCommand> command_;

    void focusFirstItem();
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onCommandTextChanged(const QString& text);
};
