#pragma once

#include <QFrame>

#include "search_pattern.h"

#include "ui_SearchWidget.h"

class SearchWidget: public QFrame, private Ui::SearchWidget {
    Q_OBJECT

signals:
    void searchNext();
    void searchPatternChanged(const SearchPattern&);

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

    bool isPatternValid() const;
    SearchPattern getSearchPattern() const;

    void setState(State state);

private slots:
    void onReturnPressed();
    void onSearchRegExpChanged();

private:
    int mode_;

    void updateLabels();
    void updateWidgets();
};
