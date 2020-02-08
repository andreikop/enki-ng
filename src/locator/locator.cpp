#include <QDebug>
#include <QVariant>
#include <QStyledItemDelegate>
#include <QPainter>

#include "locator.h"

#include "core.h"

#include "open_file_command.h"

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

};  // anonymous namespace


LocatorDialog::LocatorDialog(QMainWindow* parent):
        QDialog(parent)
{
    setupUi(this);
    QFont biggerFont = font();
    biggerFont.setPointSizeF(biggerFont.pointSizeF() * 2);

    lineEdit->setFont(biggerFont);
    listView->setFont(biggerFont);

    int width = QFontMetrics(biggerFont).width(QString().fill('x', 64));  // width of 64 'x' letters
    resize(width, width * 0.62);

    command_ = std::make_unique<OpenFileCommand>();
    listView->setModel(&command_->model());
    listView->setItemDelegate(new HTMLDelegate(listView));

    connect(lineEdit, &QLineEdit::textChanged, command_.get(), &OpenFileCommand::setCommandText);

    connect(listView, &QListView::activated, command_.get(), &OpenFileCommand::onItemActivated);
    connect(command_.get(), &OpenFileCommand::done, this, [this] {this->done(0);});
}

Locator::Locator():
    showAction_(QIcon::fromTheme("go-jump"), "Locator", this)
{
    connect(&showAction_, &QAction::triggered, this, &Locator::onTriggered);
    showAction_.setShortcut(QKeySequence("Ctrl+L"));

    core().mainWindow().menuBar()->goToMenu()->addAction(&showAction_);
}

void Locator::onTriggered() {
    LocatorDialog locatorDialog(&core().mainWindow());
    locatorDialog.exec();
}
