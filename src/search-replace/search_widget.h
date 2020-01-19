#pragma once

#include <QFrame>

#include "ui_SearchWidget.h"

class SearchWidget: public QFrame, private Ui::SearchWidget {
public:
    enum State {
        NORMAL,
        BAD,  // not found
        GOOD,  // found something
        INCORRECT  // incorrect reg exp
    };

    explicit SearchWidget();

    void setMode(int mode);
    void updateComboBoxes();

    QRegularExpression getRegExp() const;

    void setState(State state);

private:
    int mode_;

    void updateLabels();
    void updateWidgets();
};
