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

    struct Match {
        Match(int start, int end):
            start(start),
            end(end)
        {};

        Match():
            start(-1),
            end(-1)
        {};

        int start;
        int end;
    };

    struct SearchResult {
        SearchResult():
            matchIndex(-1),
            matchCount(0)
        {}

        bool isValid() {
            return cursor != QTextCursor();
        }

        QTextCursor cursor;
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
    void updateFileActionsState();
    void searchFile(Direction forward, IncrementalMode incremental);
    QVector<Match> findAll(Qutepart::Qutepart* qpart, const QRegularExpression& regExp) const;
    int chooseMatch(const QVector<Match>& matches, int cursorPos, SearchController::Direction direction) const;
    SearchResult searchInText(
            Qutepart::Qutepart* qpart,
            const QRegularExpression& regExp,
            int startPoint, Direction direction);

    QAction* searchAction_;
    QAction* searchPrev_;
    QAction* searchNext_;

    int mode_;
    int searchInFileLastCursorPos_;
    int searchInFileStartPoint_;

    std::unique_ptr<SearchWidget> searchWidget_;
};
