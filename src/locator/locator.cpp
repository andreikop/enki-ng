#include <QDebug>
#include <QVariant>

#include "locator.h"

#include "core.h"

#include "open_file_command.h"


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

    connect(lineEdit, &QLineEdit::textChanged, command_.get(), &OpenFileCommand::setCommandText);
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
