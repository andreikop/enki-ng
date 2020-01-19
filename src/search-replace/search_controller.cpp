#include <QStatusBar>
#include <QDebug>

#include "core.h"

#include "search_controller.h"
#include "search_flags.h"


SearchController::SearchController():
    mode_(0),
    searchInFileLastCursorPos_(-1),
    searchInFileStartPoint_(-1)
{
    createActions();

    connect(
        core()->workspace(), &Workspace::currentEditorChanged,
        this, &SearchController::updateFileActionsState);
}


SearchController::~SearchController() {

}

QAction* SearchController::createAction(
        const QString& text,
        const QString& icon,
        const QString& shortcut,
        const QString& toolTip,
        bool enabled) {
    QMenu* searchMenu = core()->mainWindow()->menuBar()->searchMenu();
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

    bool haveDocument = core()->workspace()->currentEditor() != nullptr;
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
    if ( ! bool(searchWidget_)) {
        searchWidget_ = std::make_unique<SearchWidget>();
        core()->mainWindow()->setSearchWidget(searchWidget_.get());
        searchWidget_->setVisible(false);
    }

    if (newMode & MODE_FLAG_FILES &&
        core()->workspace()->editors().isEmpty()) {
            return;
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

// Search Next clicked
void SearchController::onSearchNext() {
    searchWidget_->updateComboBoxes();
    searchFile(FORWARD, NON_INCREMENTAL);
}

void SearchController::onSearchPrevious() {
    searchWidget_->updateComboBoxes();
    searchFile(BACKWARD, INCREMENTAL);
}

void SearchController::searchFile(Direction direction, IncrementalMode incremental) {
    Qutepart::Qutepart* qutepart = &core()->workspace()->currentEditor()->qutepart();

    QRegularExpression regExp = searchWidget_->getRegExp();

    if (qutepart->textCursor().position() != searchInFileLastCursorPos_) {
        searchInFileStartPoint_ = -1;
    }

    if (searchInFileStartPoint_ != -1 || ( ! incremental == INCREMENTAL)) {
        // get cursor position
        QTextCursor cursor = qutepart->textCursor();

        if (direction == FORWARD) {
            if (incremental == INCREMENTAL) {
                searchInFileStartPoint_ = cursor.selectionStart();
            } else {
                searchInFileStartPoint_ = cursor.selectionEnd();
            }
        } else {
            searchInFileStartPoint_ = cursor.selectionStart();
        }
    }

    SearchController::SearchResult res = searchInText(qutepart, regExp, searchInFileStartPoint_, direction);

    if (res.isValid()) {
        qutepart->setTextCursor(res.cursor);
        searchInFileLastCursorPos_ = res.cursor.anchor();
        searchWidget_->setState(SearchWidget::GOOD);  // change background acording to result
        core()->mainWindow()->statusBar()->showMessage(
            QString("Match %1 of %2").arg(res.matchIndex + 1).arg(res.matchCount), 3000);
    } else {
        searchWidget_->setState(SearchWidget::BAD);

        // Reset selection
        QTextCursor cursor = qutepart->textCursor();
        cursor.setPosition(cursor.position());
        qutepart->setTextCursor(cursor);
    }
}

QVector<SearchController::Match> SearchController::findAll(
        Qutepart::Qutepart* qpart,
        const QRegularExpression& regExp) const {
    QTextCursor cursor = QTextCursor(qpart->document());
    QVector<Match> result;

    while (1) {
        cursor = qpart->document()->find(regExp, cursor);
        if (cursor == QTextCursor()) {
            break;
        } else {
            result.append(Match(cursor.anchor(), cursor.position()));
        }
    }

    return result;
}

int SearchController::chooseMatch(
        const QVector<SearchController::Match>& matches,
        int cursorPos,
        SearchController::Direction direction) const {
    int index = 0;
    for(; index < matches.length(); index++) {
        if (std::min(matches[index].start, matches[index].end) >= cursorPos) {
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
        const QRegularExpression& regExp,
        int startPoint, Direction direction) {
    SearchController::SearchResult res;
    QVector<Match> matches = findAll(qpart, regExp);

    if ( ! matches.isEmpty()) {
        int startPos = -1;
        QTextCursor currentCursor = qpart->textCursor();
        if (direction == FORWARD) {
            startPos = std::max(currentCursor.position(), currentCursor.anchor());
        } else {
            startPos = std::min(currentCursor.position(), currentCursor.anchor());
        }

        res.matchIndex = chooseMatch(matches, startPos, direction);

        res.cursor = qpart->textCursor();
        res.cursor.setPosition(matches[res.matchIndex].start);
        res.cursor.setPosition(matches[res.matchIndex].end, QTextCursor::KeepAnchor);

        res.matchCount = matches.length();
    }

    return res;
}
