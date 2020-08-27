#pragma once

#include <memory>

#include <QAction>

#include "core/module.h"


class Zoom: public Module {
    Q_OBJECT
public:
    Zoom();

private slots:
    void onZoomIn();
    void onZoomOut();
    void onCurrentEditorChanged(Editor* editor);

private:
    void zoom(double multiplier);
    std::shared_ptr<QAction> zoomInAction_;
    std::shared_ptr<QAction> zoomOutAction_;
};
