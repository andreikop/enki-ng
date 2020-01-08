#pragma once

#include <memory>

#include <QMenu>
#include <QAction>

#include "search_widget.h"

class SearchController: public QObject {
    Q_OBJECT

public:
    SearchController();
    ~SearchController();

private slots:
    void onModeSwitchTriggered(int mode);
    void onSearchNext();
    void onSearchPrevious();

private:
    QAction* createAction(
        const QString& text,
        const QString& icon,
        const QString& shortcut,
        const QString& toolTip,
        bool enabled=true);
    void createActions();

    QAction* searchAction_;

    std::unique_ptr<SearchWidget> searchWidget_;
};
