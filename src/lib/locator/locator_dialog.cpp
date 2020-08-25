#include <QVariant>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>

#include "locator_dialog.h"


namespace {

/* QStyledItemDelegate implementation. Draws HTML

http://stackoverflow.com/questions/1956542/how-to-make-item-view-render-rich-html-text-in-qt/1956781#1956781
*/
class HTMLDelegate: public QStyledItemDelegate {
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(
            QPainter *painter,
            const QStyleOptionViewItem& option,
            const QModelIndex& index) const {
        QStyleOptionViewItem myOption(option);
        myOption.state &= ~QStyle::State_HasFocus;  // never draw focus rect

        QStyleOptionViewItem options(myOption);
        initStyleOption(&options, index);

        QStyle* style = nullptr;
        if (options.widget == nullptr) {
            style = QApplication::style();
        } else {
            style = options.widget->style();
        }

        QTextDocument doc;
        doc.setDocumentMargin(1);
        doc.setHtml(options.text);

        if (options.widget != nullptr) {
            doc.setDefaultFont(options.widget->font());
        }
        // bad long (multiline) strings processing doc.setTextWidth(options.rect.width())

        options.text = "";
        style->drawControl(QStyle::CE_ItemViewItem, &options, painter);

        QAbstractTextDocumentLayout::PaintContext ctx;

        // Highlighting text if item is selected
        if (myOption.state & QStyle::State_Selected) {
            ctx.palette.setColor(
                    QPalette::Text,
                    myOption.palette.color(QPalette::Active, QPalette::HighlightedText));
        }

        QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &options);
        painter->save();
        painter->translate(textRect.topLeft());

        /* Original example contained line
            painter.setClipRect(textRect.translated(-textRect.topLeft()))
        but text is drawn clipped with it on kubuntu 12.04
        */

        doc.documentLayout()->draw(painter, ctx);

        painter->restore();
    }

    QSize sizeHint(
            const QStyleOptionViewItem &option,
            const QModelIndex &index) const {
        QStyleOptionViewItem options(option);
        initStyleOption(&options, index);

        QTextDocument doc;
        doc.setDocumentMargin(1);
        //  bad long (multiline) strings processing doc.setTextWidth(options.rect.width())
        doc.setHtml(options.text);
        return QSize(doc.idealWidth(),
                     QStyledItemDelegate::sizeHint(option, index).height());
    }
};

}  // anonymous namespace


LocatorDialog::LocatorDialog(QMainWindow* parent, std::shared_ptr<LocatorCommand> command):
        QDialog(parent),
        command_(command)
{
    setupUi(this);
    QFont biggerFont = font();
    biggerFont.setPointSizeF(biggerFont.pointSizeF() * 2);

    lineEdit->setFont(biggerFont);
    listView->setFont(biggerFont);

    listView->setFocusProxy(lineEdit);

    lineEdit->installEventFilter(this);  // forward Up, Down to listView

    int width = QFontMetrics(biggerFont).width(QString().fill('x', 64));  // width of 64 'x' letters
    resize(width, width * 0.62);

    listView->setModel(&command_->model());
    listView->setItemDelegate(new HTMLDelegate(listView));

    connect(lineEdit, &QLineEdit::textChanged, this, &LocatorDialog::onCommandTextChanged);

    connect(listView, &QListView::activated, command_.get(), &LocatorCommand::onItemActivated);
    connect(command_.get(), &LocatorCommand::done, this, [this] {this->done(0);});

    focusFirstItem();
}

bool LocatorDialog::eventFilter(QObject *obj, QEvent *event) {
    if (obj == lineEdit &&
        event->type() == QEvent::KeyPress) {
        switch (dynamic_cast<QKeyEvent*>(event)->key()) {
            case Qt::Key_Up:
            case Qt::Key_Down:
            case Qt::Key_PageUp:
            case Qt::Key_PageDown:
                 return QApplication::sendEvent(listView, event);
            case Qt::Key_Return:  // Execute the dialogue
            case Qt::Key_Enter:
                // Execute the dialogue
                command_->onItemActivated(listView->currentIndex());
                return true;
            default:
                return false;
        }
    }

    return false;
}

void LocatorDialog::focusFirstItem() {
    listView->setCurrentIndex(listView->model()->index(0, 0));  // return focus to the first item
}

void LocatorDialog::onCommandTextChanged(const QString& text) {
    command_->setCommandText(text);
    focusFirstItem();
}
