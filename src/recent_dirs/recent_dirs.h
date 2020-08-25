#pragma once

#include <QAction>

#include "module.h"


class RecentDirs: public Module {
    Q_OBJECT
public:
    RecentDirs();

private slots:
    void onRecentDirsTriggered();
    void onDirectorySwitched(const QDir& path);
};
