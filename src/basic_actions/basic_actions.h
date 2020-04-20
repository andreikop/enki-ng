#pragma once

#include "editor.h"
#include "module.h"


class BasicActions: public Module {
    Q_OBJECT
public:
    BasicActions();

private slots:
    void onSwitchDirectoryAction();
    void onFileOpen();
    void onFileSave();
    void updateEditMenuActions(Editor* editor);
};
