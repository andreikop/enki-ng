#include <QTreeView>

#include "file_explorer.h"

FileExplorer::FileExplorer(QMainWindow *mainWindow):
    QDockWidget("Opened files", mainWindow)
{
    setAllowedAreas(Qt::LeftDockWidgetArea);
    setFeatures(QDockWidget::DockWidgetClosable);


    QTreeView* treeView = new QTreeView(this);
    treeView->setHeaderHidden(true);
    treeView->setRootIsDecorated(false);
    treeView->setTextElideMode(Qt::ElideMiddle);
    treeView->setUniformRowHeights(true);

    setWidget(treeView);
    mainWindow->addDockWidget(Qt::LeftDockWidgetArea, this);
}
