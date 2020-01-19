#pragma once

#include <memory>

#include <QMenu>
#include <QAction>

#include "qutepart/qutepart.h"

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
    enum Direction {
        FORWARD,
        BACKWARD
    };

    enum IncrementalMode {
        INCREMENTAL,
        NON_INCREMENTAL
    };

    struct SearchResult {
        SearchResult():
            matchStart(-1),
            matchEnd(-1),
            matchIndex(-1),
            matchCount(0)
        {}

        bool isValid() {
            return matchStart != -1;
        }

        int matchStart;
        int matchEnd;
        int matchIndex;
        int matchCount;
    };

    QAction* createAction(
        const QString& text,
        const QString& icon,
        const QString& shortcut,
        const QString& toolTip,
        bool enabled=true);
    void createActions();
    void searchFile(Direction forward, IncrementalMode incremental);
    SearchResult searchInText(
            const QRegularExpression& regExp,
            Qutepart::Qutepart* qpart,
            int startPoint, Direction direction);

    QAction* searchAction_;
    int mode_;
    int searchInFileLastCursorPos_;
    int searchInFileStartPoint_;

    std::unique_ptr<SearchWidget> searchWidget_;
};
