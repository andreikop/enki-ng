#include <QDebug>

#include "locator.h"

#include "core.h"


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
}

Locator::Locator():
    showAction_(QIcon::fromTheme("go-jump"), "Locator", this),
    widget_(&core().mainWindow())
{
    connect(&showAction_, &QAction::triggered, &widget_, &LocatorDialog::open);
    showAction_.setShortcut(QKeySequence("Ctrl+L"));

    core().mainWindow().menuBar()->goToMenu()->addAction(&showAction_);
}
