#include "core/core.h"
#include "core/main_window.h"

#include "locator_dialog.h"
#include "locator.h"

Locator::Locator():
    showAction_(QIcon::fromTheme("go-jump"), "Go to file in project...", this)
{
    connect(&showAction_, &QAction::triggered, this, &Locator::onTriggered);
    showAction_.setShortcut(QKeySequence("Ctrl+P"));

    core().mainWindow().menuBar()->navigationMenu()->addAction(&showAction_);
}

void Locator::onTriggered() {
    LocatorDialog locatorDialog(&core().mainWindow());
    locatorDialog.exec();
}
