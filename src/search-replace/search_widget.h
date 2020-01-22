#pragma once

#include <QFrame>

#include "ui_SearchWidget.h"

class SearchWidget: public QFrame, private Ui::SearchWidget {
    Q_OBJECT

signals:
    void searchNext();
    void searchRegExpChanged(const QRegularExpression&);

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

private slots:
    void onReturnPressed();
    void onSearchRegExpChanged();

private:
    int mode_;

    void updateLabels();
    void updateWidgets();
};
