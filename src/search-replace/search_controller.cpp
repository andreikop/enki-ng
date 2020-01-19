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

    QAction* searchPrev  = createAction(
        "Search &Previous",
        "previous.png", "Shift+F3",
        "Search previous occurrence",
        false);
    connect(searchPrev, &QAction::triggered, this, &SearchController::onSearchPrevious);

    QAction* searchNext = createAction(
        "Search &Next",
        "next.png", "F3",
        "Search next occurrence",
        false);
    connect(searchNext, &QAction::triggered, this, &SearchController::onSearchNext);
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

#if 0
    QRegularExpression regExp = widget->getRegExp();
#endif

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

#if 0
    match, matches = self._searchInText(regExp, qutepart.text, self._searchInFileStartPoint, forward)
    if match:
        selectionStart, selectionEnd = match.start(), match.start() + len(match.group(0))
        qutepart.absSelectedPosition = (selectionStart, selectionEnd)
        self._searchInFileLastCursorPos = selectionEnd
        self._widget.setState(self._widget.Good)  # change background acording to result
        core.mainWindow().statusBar().showMessage('Match %d of %d' %
                                                  (matches.index(match) + 1, len(matches)), 3000)
    else:
        self._widget.setState(self._widget.Bad)
        qutepart.resetSelection()
#endif
}

SearchController::SearchResult SearchController::searchInText(
        const QRegularExpression& regExp,
        Qutepart::Qutepart* qpart,
        int startPoint, Direction direction) {
    // TODO
}
