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
Doesn't escape space, comma, underscorer
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

    updateLabels();
    updateWidgets();
}

namespace {
void addCurrentValToHistory(QComboBox* combo) {
    QString text = combo->currentText();
    if ( ! text.isEmpty()) {
        int index = combo->findText(text);

        if (index == -1) {
            combo->addItem(text);
        }
    }
}
};

// Update comboboxes with last used texts
void SearchWidget::updateComboBoxes() {
    addCurrentValToHistory(cbSearch);
    addCurrentValToHistory(cbReplace);
    addCurrentValToHistory(cbMask);
}

QRegularExpression SearchWidget::getRegExp() const {
    QString pattern = cbSearch->currentText();

    // replace unicode paragraph separator with habitual \n
    pattern = pattern.replace("\u2029", "\n");

    if ( ! cbRegularExpression->checkState() == Qt::Checked) {
        pattern = QRegularExpression::escape(pattern);
    }

    if (cbWholeWord->checkState() == Qt::Checked) {
        pattern = QString("\b" + pattern + "\b");
    }

    QRegularExpression::PatternOption flags = QRegularExpression::NoPatternOption;
    if (cbCaseSensitive->checkState() != Qt::Checked) {
        flags = QRegularExpression::CaseInsensitiveOption;
    }

    return QRegularExpression(pattern, flags);
}

void SearchWidget::setState(State state) {
    QWidget* widget = cbSearch->lineEdit();

    QColor color;

    switch (state) {
        case NORMAL:
            color = QGuiApplication::palette().color(QPalette::Base);
            break;
        case GOOD:
            color = Qt::green;
            break;
        case BAD:
            color = Qt::red;
            break;
        case INCORRECT:
            color = Qt::darkYellow;
            break;
    }

    if (state != NORMAL) {
        color.setAlpha(100);
    }

    QPalette pal = widget->palette();
    pal.setColor(widget->backgroundRole(), color);
    widget->setPalette(pal);
}

// Update 'Search' 'Replace' 'Path' labels geometry
void SearchWidget::updateLabels() {
    int width = 0;

    if (lSearch->isVisible()) {
        width = std::max(width, lSearch->minimumSizeHint().width());
    }

    if (lReplace->isVisible()) {
        width = std::max(width, lReplace->minimumSizeHint().width());
    }

    if (lPath->isVisible()) {
        width = std::max(width, lPath->minimumSizeHint().width());
    }

    lSearch->setMinimumWidth(width);
    lReplace->setMinimumWidth(width);
    lPath->setMinimumWidth(width);
}

// Update geometry of widgets with buttons
void SearchWidget::updateWidgets() {
    int width = 0;

    if (wSearchRight->isVisible()) {
        width = std::max(width, wSearchRight->minimumSizeHint().width());
    }

    if (wReplaceRight->isVisible()) {
        width = std::max(width, wReplaceRight->minimumSizeHint().width());
    }

    if (wPathRight->isVisible()) {
        width = std::max(width, wPathRight->minimumSizeHint().width());
    }

    wSearchRight->setMinimumWidth(width);
    wReplaceRight->setMinimumWidth(width);
    wPathRight->setMinimumWidth(width);
}
