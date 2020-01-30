#include <QDebug>

#include "locator.h"

#include "core.h"


class OpenFileCommand {
public:
    QStringList fileList() const {
        return core().project().fileList();
    }
};

namespace {

class LocatorModel: public QAbstractItemModel {
public:
    LocatorModel(QObject* parent, const OpenFileCommand& command):
        QAbstractItemModel(parent),
        command_(command)
    {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        if (parent.isValid()) {
            return 0;
        } else {
            return command_.fileList().size();
        }
    }

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

    command_ = std::make_unique<OpenFileCommand>();
    listView->setModel(new LocatorModel(this, *command_));
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
