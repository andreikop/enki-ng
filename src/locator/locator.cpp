#include "core.h"
#include "main_window.h"
#include "locator_dialog.h"

#include "locator.h"

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
