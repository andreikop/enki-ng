#pragma once

#include <memory>

#include <QDialog>
#include <QMainWindow>

#include "locator_model.h"
#include "open_file_command.h"

#include "ui_LocatorDialog.h"


class LocatorDialog: public QDialog, private Ui_LocatorDialog {
public:
    LocatorDialog(QMainWindow* parent);

private:
    std::unique_ptr<OpenFileCommand> command_;

    void focusFirstItem();
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onCommandTextChanged(const QString& text);
};
