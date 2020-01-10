#include <QDebug>

#include "core.h"

#include "search_controller.h"
#include "search_flags.h"


SearchController::SearchController() {
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

        self._mode = newMode

        if self._dock is not None:
            self._dock.setReplaceMode(self._mode == MODE_REPLACE_DIRECTORY or
                                      self._mode == MODE_REPLACE_OPENED_FILES)
#endif
}

void SearchController::onSearchNext() {

}

void SearchController::onSearchPrevious() {

}
