#include <QStatusBar>
#include <QDebug>

#include "core/core.h"
#include "core/workspace.h"

#include "search_controller.h"
#include "search_flags.h"


SearchController::SearchController():
    mode_(0),
    searchInFileLastCursorPos_(-1),
    searchInFileStartPoint_(-1)
{
    createActions();

    connect(
        &core().workspace(), &Workspace::currentEditorChanged,
        this, &SearchController::updateFileActionsState);
}


SearchController::~SearchController() {

}

QAction* SearchController::createAction(
        const QString& text,
        const QString& /*icon*/,
        const QString& shortcut,
        const QString& toolTip,
        bool enabled) {
    QMenu* searchMenu = core().mainWindow().menuBar()->searchMenu();
    QAction* action = searchMenu->addAction(text);
    if ( ! shortcut.isNull()) {
        action->setShortcut(shortcut);
    }
    if ( ! toolTip.isNull()) {
        action->setToolTip(toolTip);
    }

    // TODO icon

    action->setEnabled(enabled);

    return action;
}

void SearchController::createActions() {
#if 0
        if sys.platform == 'darwin':
            # Ctrl+, conflicts with "Open preferences"
            searchWordBackwardShortcut, searchWordForwardShortcut = 'Meta+,', 'Meta+.'
        else:
            searchWordBackwardShortcut, searchWordForwardShortcut = 'Ctrl+,', 'Ctrl+.'
#endif

    // List if search actions.
    // First acition created by MainWindow, so, do not fill text

    searchAction_ = createAction(
        "&Search...",
        "search.png", "Ctrl+F",
        "Search in the current file...");
    connect(searchAction_, &QAction::triggered, [this]{this->onModeSwitchTriggered(MODE_SEARCH);});

    searchPrev_  = createAction(
        "Search &Previous",
        "previous.png", "Shift+F3",
        "Search previous occurrence",
        false);
    connect(searchPrev_, &QAction::triggered, this, &SearchController::onSearchPrevious);

    searchNext_ = createAction(
        "Search &Next",
        "next.png", "F3",
        "Search next occurrence",
        false);
    connect(searchNext_, &QAction::triggered, this, &SearchController::onSearchNext);
#if 0
    createAction("aReplaceFile", "&Replace...",
                 "replace.png", "Ctrl+R",
                 "Replace in the current file...",
                 self._onModeSwitchTriggered, MODE_REPLACE)
    createAction("aSearchWordBackward", "Search word under cursor backward",
                 "less.png", searchWordBackwardShortcut,
                 "",
                 self._onSearchCurrentWordBackward, None)
    createAction("aSearchWordForward", "Search word under cursor forward",
                 "bigger.png", searchWordForwardShortcut,
                 "",
                 self._onSearchCurrentWordForward, None)
    self._menuSeparator = core.actionManager().menu(menu).addSeparator()
    createAction("aSearchDirectory", "Search in &Directory...",
                 "search-replace-directory.png", "Ctrl+Shift+F",
                 "Search in directory...",
                 self._onModeSwitchTriggered, MODE_SEARCH_DIRECTORY)
    createAction("aReplaceDirectory", "Replace in Director&y...",
                 "search-replace-directory.png", "Ctrl+Shift+R",
                 "Replace in directory...",
                 self._onModeSwitchTriggered, MODE_REPLACE_DIRECTORY)
    createAction("aSearchOpenedFiles", "Search in &Opened Files...",
                 "search-replace-opened-files.png",
                 "Ctrl+Alt+F", "Search in opened files...",
                 self._onModeSwitchTriggered, MODE_SEARCH_OPENED_FILES)
    createAction("aReplaceOpenedFiles", "Replace in Open&ed Files...",
                 "search-replace-opened-files.png", "Ctrl+Alt+R",
                 "Replace in opened files...",
                 self._onModeSwitchTriggered, MODE_REPLACE_OPENED_FILES)
#endif
}

void SearchController::updateFileActionsState() {
#if 0
    valid, error = self._widget.isSearchRegExpValid()
    valid = valid and len(self._widget.getRegExp().pattern) > 0  # valid and not empty
#else
    bool valid = true;
#endif

    bool haveDocument = core().workspace().currentEditor() != nullptr;
    bool searchInFileAvailable = valid and haveDocument;

#if 0
    self._widget.setSearchInFileActionsEnabled(searchInFileAvailable)
#endif
    searchNext_->setEnabled(searchInFileAvailable);
    searchPrev_->setEnabled(searchInFileAvailable);

#if 0
    core.actionManager().action("mNavigation/mSearchReplace/aSearchWordBackward").setEnabled(haveDocument)
    core.actionManager().action("mNavigation/mSearchReplace/aSearchWordForward").setEnabled(haveDocument)
#endif
}

void SearchController::onModeSwitchTriggered(int newMode) {
    if ((newMode & MODE_FLAG_FILE) &&
        core().workspace().editors().isEmpty()) {
            return;
    }

    if ( ! bool(searchWidget_)) {
        createSearchWidget();
    }

    searchWidget_->setMode(newMode);

#if 0
        if self._searchThread is not None:
            self._searchThread.stop()
        if self._replaceThread is not None:
            self._replaceThread.stop()


        if self._dock is not None:
            self._dock.setReplaceMode(self._mode == MODE_REPLACE_DIRECTORY or
                                      self._mode == MODE_REPLACE_OPENED_FILES)
#endif
    mode_ = newMode;
}

void SearchController::createSearchWidget() {
    searchWidget_ = std::make_unique<SearchWidget>();
    core().mainWindow().setSearchWidget(searchWidget_.get());

    connect(searchWidget_.get(), &SearchWidget::searchNext, this, &SearchController::onSearchNext);
    connect(searchWidget_.get(), &SearchWidget::searchPatternChanged,
            this, &SearchController::onSearchPatternChanged);

    searchWidget_->setVisible(false);
}

// Search regExp changed. Do incremental search
void SearchController::onSearchPatternChanged(const SearchPattern& pattern) {
    if ( ! pattern.isValid()) {
        core().mainWindow().statusBar()->showMessage(pattern.regExp().errorString(), 3000);
        searchWidget_->setState(SearchWidget::INCORRECT);
    } else if ( (mode_ == MODE_SEARCH || mode_ == MODE_REPLACE) &&
               core().workspace().currentEditor() != nullptr) {
        if (pattern.isEmpty()) {
            core().workspace().currentEditor()->qutepart().resetSelection();
        } else {  // Clear selection
            searchFile(FORWARD, INCREMENTAL);
        }
    }
}

// Search Next clicked
void SearchController::onSearchNext() {
    searchWidget_->updateComboBoxes();
    searchFile(FORWARD, NON_INCREMENTAL);
}

void SearchController::onSearchPrevious() {
    searchWidget_->updateComboBoxes();
    searchFile(BACKWARD, NON_INCREMENTAL);
}

void SearchController::searchFile(Direction direction, IncrementalMode incrementalMode) {
    Qutepart::Qutepart* qutepart = &core().workspace().currentEditor()->qutepart();

    SearchPattern pattern = searchWidget_->getSearchPattern();

    // Choose start point
    updateSearchStartPoint(qutepart, direction, incrementalMode);

    // Search
    SearchController::SearchResult res = searchInText(qutepart, pattern, searchInFileStartPoint_, direction);

    // Show results
    if (res.isValid()) {
        qutepart->setTextCursor(res.cursor);
        qutepart->centerCursor();
        searchInFileLastCursorPos_ = res.cursor.position();
        searchWidget_->setState(SearchWidget::GOOD);  // change background acording to result
        core().mainWindow().statusBar()->showMessage(
            QString("Match %1 of %2").arg(res.matchIndex + 1).arg(res.matchCount), 3000);
    } else {
        searchWidget_->setState(SearchWidget::BAD);

        // Reset selection
        qutepart->resetSelection();
    }
}

// Choose start point from which search started
void SearchController::updateSearchStartPoint(
        Qutepart::Qutepart* qutepart,
        Direction direction,
        IncrementalMode incrementalMode) {
    if (qutepart->textCursor().position() != searchInFileLastCursorPos_) {
        searchInFileStartPoint_ = -1;
    }

    if (searchInFileStartPoint_ == -1 || (incrementalMode == NON_INCREMENTAL)) {
        // get cursor position
        QTextCursor cursor = qutepart->textCursor();

        if (direction == FORWARD) {
            if (incrementalMode == INCREMENTAL) {
                searchInFileStartPoint_ = cursor.selectionStart();
            } else {
                searchInFileStartPoint_ = cursor.selectionEnd();
            }
        } else {
            searchInFileStartPoint_ = cursor.selectionStart();
        }
    }
}

QVector<SearchController::Match> SearchController::findAllQuick(
        Qutepart::Qutepart* qpart,
        const SearchPattern& pattern) const {
    QTextCursor cursor = QTextCursor(qpart->document());
    QVector<Match> result;

    QTextDocument::FindFlags qTexDocFlags;
    if (pattern.flags & SearchPattern::WHOLE_WORD) {
        qTexDocFlags.setFlag(QTextDocument::FindWholeWords);
    }
    if (pattern.flags & SearchPattern::CASE_SENSITIVE) {
        qTexDocFlags.setFlag(QTextDocument::FindCaseSensitively);
    }

    while (1) {
        cursor = qpart->document()->find(pattern.text, cursor, qTexDocFlags);
        if (cursor == QTextCursor()) {
            break;
        } else {
            result.append(Match(cursor.anchor(), cursor.position()));
        }
    }

    return result;
}

QVector<SearchController::Match> SearchController::findAllRegExp(
        Qutepart::Qutepart* qpart,
        const SearchPattern& pattern) const {
    QRegularExpression regExp = pattern.regExp();
    QVector<Match> result;

    QRegularExpressionMatchIterator it = regExp.globalMatch(qpart->document()->toPlainText());

    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        result.append(Match(match.capturedStart(), match.capturedEnd()));
        // qDebug() << "cap" << match.capturedStart() << match.capturedLength()
    }

    return result;
}

int SearchController::chooseMatch(
        const QVector<SearchController::Match>& matches,
        int cursorPos,
        SearchController::Direction direction) const {
    int index = 0;
    for(; index < matches.length(); index++) {
        if (matches[index].start, matches[index].end > cursorPos) {
            break;
        }
    }

    // Now all items before index are less than pos

    // wrap if reached end

    if (direction == SearchController::BACKWARD) {
        index--;
    }

    if (index == matches.length()) {
        index = 0;
    } else if (index < 0) {
        index = matches.length() - 1;
    }

    return index;
}

SearchController::SearchResult SearchController::searchInText(
        Qutepart::Qutepart* qpart,
        const SearchPattern& pattern,
        int startPoint, Direction direction) {
    QVector<Match> matches;

    if (pattern.flags & SearchPattern::REG_EXP) {
        matches = findAllRegExp(qpart, pattern);
    } else {
        matches = findAllQuick(qpart, pattern);
    }

    SearchController::SearchResult res;
    if ( ! matches.isEmpty()) {
        res.matchIndex = chooseMatch(matches, startPoint, direction);

        res.cursor = qpart->textCursor();
        res.cursor.setPosition(matches[res.matchIndex].start);
        res.cursor.setPosition(matches[res.matchIndex].end, QTextCursor::KeepAnchor);

        res.matchCount = matches.length();
    }

    return res;
}
