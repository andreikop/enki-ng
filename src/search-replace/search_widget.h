#pragma once

#include <QFrame>

#include "ui_SearchWidget.h"

class SearchWidget: public QFrame, private Ui::SearchWidget {
public:
    explicit SearchWidget();

    void setMode(int mode);
    void updateComboBoxes();

private:
    int mode_;

    void updateLabels();
    void updateWidgets();
};
