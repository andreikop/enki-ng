#include "core/core.h"
#include "core/main_window.h"

#include "file_tree.h"
#include "file_browser.h"


FileBrowserDock::FileBrowserDock(QMainWindow* mainWindow):
    QDockWidget(QString("File Browser"), mainWindow)
{
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    setFeatures(QDockWidget::DockWidgetClosable);
    mainWindow->addDockWidget(Qt::LeftDockWidgetArea, this);

    setWidget(new FileTree(this));
}

FileBrowser::FileBrowser():
    dock_(std::make_unique<FileBrowserDock>(&core().mainWindow()))
{}
