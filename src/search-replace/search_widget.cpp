#include <QLineEdit>

#include "core.h"
#include "search_widget.h"
#include "search_flags.h"


namespace {
QString makeEscapeSeqsVisible(const QString& text) {
    // TODO
    return text;
}

QString regExEscape(const QString& text) {
    // TODO
    return text;
}

}; // anonymous namespace

SearchWidget::SearchWidget():
    QFrame(core()->mainWindow()) {
    setupUi(this);
}

/* Change search mode.
i.e. from "Search file" to "Replace directory"
*/
void SearchWidget::setMode(int mode) {
    Editor* editor = core()->workspace()->currentEditor();
    if (mode_ == mode && isVisible()) {
        if (editor != nullptr &&
            (! editor->qutepart().hasFocus())) {
            cbSearch->lineEdit()->selectAll();
            cbSearch->setFocus();
        }
    }

    mode_ = mode;

    // Set Search and Replace text
    if (editor != nullptr &&
        editor->qutepart().hasFocus() &&
        ( ! editor->qutepart().textCursor().selectedText().isEmpty())) {
        QString searchText = editor->qutepart().textCursor().selectedText();

        cbReplace->setEditText(makeEscapeSeqsVisible(searchText));

        if (cbRegularExpression->checkState() == Qt::Checked) {
            searchText = regExEscape(searchText);
        }
        cbSearch->setEditText(searchText);
    }

    if (cbReplace->lineEdit()->text().isEmpty() &&
        ( ! cbSearch->lineEdit()->text().isEmpty()) &&
        cbRegularExpression->checkState() != Qt::Checked) {
        QString replaceText = cbSearch->lineEdit()->text().replace("\\", "\\\\");
        cbReplace->setEditText(replaceText);
    }

    // Move focus to Search edit
    cbSearch->setFocus();
    cbSearch->lineEdit()->selectAll();

#if 0
    // Set search path
    if (mode & MODE_FLAG_DIRECTORY &&
        ( ! (isVisible() && cbPath->isVisible()))) {

        try:
            searchPath = os.path.abspath(str(os.path.curdir))
        except OSError:  # current directory might have been deleted
            pass
        else:
            self.cbPath.setEditText(searchPath)
    }
    # Set widgets visibility flag according to state
    widgets = (self.wSearch, self.pbPrevious, self.pbNext, self.pbSearch, self.wReplace, self.wPath,
               self.pbReplace, self.pbReplaceAll, self.pbReplaceChecked, self.wOptions, self.wMask)
    #                         wSear  pbPrev pbNext pbSear wRepl  wPath  pbRep  pbRAll pbRCHK wOpti wMask
    visible = \
        {MODE_SEARCH:               (1,     1,     1,     0,     0,     0,     0,     1,     1,    1,    0,),
         MODE_REPLACE:               (1,     1,     1,     0,     1,     0,     1,     1,     0,    1,    0,),
         MODE_SEARCH_DIRECTORY:      (1,     0,     0,     1,     0,     1,     0,     0,     0,    1,    1,),
         MODE_REPLACE_DIRECTORY:     (1,     0,     0,     1,     1,     1,     0,     0,     1,    1,    1,),
         MODE_SEARCH_OPENED_FILES:   (1,     0,     0,     1,     0,     0,     0,     0,     0,    1,    1,),
         MODE_REPLACE_OPENED_FILES:  (1,     0,     0,     1,     1,     0,     0,     0,     1,    1,    1,)}

    for i, widget in enumerate(widgets):
        widget.setVisible(visible[mode][i])
#endif

    // Search next button text
    if (mode == MODE_REPLACE) {
        pbNext->setText("Next");
    } else {
        pbNext->setText("Next↵");
    }

    // Finaly show all with valid size
    show();  // show before updating widgets and labels

#if 0
    self._updateLabels()
    self._updateWidgets()
#endif
}
