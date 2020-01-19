#include <QDir>
#include <QLineEdit>

#include "core.h"
#include "search_widget.h"
#include "search_flags.h"


namespace {

QString makeEscapeSeqsVisible(QString text) {
    return text
        .replace('\\', "\\\\")
        .replace('\t', "\\t")
        .replace('\n', "\\n");
}

/* Improved version of re.escape()
Doesn't escape space, comma, underscore.
Escapes \n and \t
*/
QString regExEscape(QString text) {
    text = QRegularExpression::escape(text);

    // re.escape escapes space, comma, underscore, but, it is not necessary and makes text not readable
    foreach(QChar symbol, QString(" ,_=\'\"/:@#%&")) {
        text = text.replace(QString("\\") + symbol, symbol);
    }

    text = text.replace("\\\n", "\\n");
    text = text.replace("\\\t", "\\t");

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

    // Set search path
    if (mode & MODE_FLAG_DIRECTORY &&
        ( ! (isVisible() && cbPath->isVisible()))) {

        QString searchPath = QDir::currentPath();
        if ( ! searchPath.isEmpty()) {  // could be empty if current dir was deleted
            cbPath->setEditText(searchPath);
        }
    }

    QVector<int> flags;
    //               wSear  pbPrev pbNext pbSear wRepl  wPath  pbRep  pbRAll pbRCHK wOpti wMask
    switch (mode) {
        case MODE_SEARCH:
            flags = {1,     1,     1,     0,     0,     0,     0,     1,     1,    1,    0};
        break;
        case MODE_REPLACE:
            flags = {1,     1,     1,     0,     1,     0,     1,     1,     0,    1,    0};
        break;
        case MODE_SEARCH_DIRECTORY:
            flags = {1,     0,     0,     1,     0,     1,     0,     0,     0,    1,    1};
        break;
        case MODE_REPLACE_DIRECTORY:
            flags = {1,     0,     0,     1,     1,     1,     0,     0,     1,    1,    1};
        break;
        case MODE_SEARCH_OPENED_FILES:
            flags = {1,     0,     0,     1,     0,     0,     0,     0,     0,    1,    1};
        break;
        case MODE_REPLACE_OPENED_FILES:
            flags = {1,     0,     0,     1,     1,     0,     0,     0,     1,    1,    1};
        break;
    }

    // Set widgets visibility flag according to state
    wSearch->setVisible(flags[0]);
    pbPrevious->setVisible(flags[1]);
    pbNext->setVisible(flags[2]);
    pbSearch->setVisible(flags[3]);
    wReplace->setVisible(flags[4]);
    wPath->setVisible(flags[5]);
    pbReplace->setVisible(flags[6]);
    pbReplaceAll->setVisible(flags[7]);
    pbReplaceChecked->setVisible(flags[8]);
    wOptions->setVisible(flags[9]);
    wMask->setVisible(flags[10]);

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
