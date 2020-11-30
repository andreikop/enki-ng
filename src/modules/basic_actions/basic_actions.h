#pragma once

#include "core/module.h"
#include "core/editor.h"


class BasicActions: public Module {
    Q_OBJECT
public:
    BasicActions();

private slots:
    void onSwitchDirectoryAction();
    void onFileNew();
    void onFileOpen();
    void onFileSave();
    void updateEditMenuActions(Editor* editor);
};
