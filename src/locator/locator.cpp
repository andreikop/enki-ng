#include <QDebug>

#include "locator.h"

#include "core.h"


namespace {

class OpenFileCommand {
public:
    QStringList fileList() const {
#if 0
        return core().project().fileList();
#else
        QStringList result;
        result << "foo";
        result << "bar";
#endif
        return result;
    }
};

class LocatorModel: public QAbstractItemModel {
public:
    LocatorModel(const OpenFileCommand& command):
        command_(command)
    {}

    int columnCount(const QModelIndex &parent = QModelIndex()) const override {
        return 1;
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (role == Qt::DisplayRole) {
            return command_.fileList()[index.row()];
        } else {
            return QVariant();  // TODO return file icon
        }
    }

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override {
        return createIndex(row, column);
    }

    QModelIndex parent(const QModelIndex &index) const override {
        return QModelIndex();
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        return command_.fileList().size();
    }

private:
    const OpenFileCommand& command_;
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

    // FIXME 2 memory leaks!!!
    listView->setModel(new LocatorModel(*(new OpenFileCommand)));
}

Locator::Locator():
    showAction_(QIcon::fromTheme("go-jump"), "Locator", this),
    widget_(&core().mainWindow())
{
    connect(&showAction_, &QAction::triggered, &widget_, &LocatorDialog::open);
    showAction_.setShortcut(QKeySequence("Ctrl+L"));

    core().mainWindow().menuBar()->goToMenu()->addAction(&showAction_);
}
