#include <QFont>

#include "main_window.h"

MainWindow::MainWindow() {
    setMenuBar(new MenuBar(this));
    resize(800, 600);

    QFont font = this->font();
    font.setPointSize(12);
    font.setFamily("Monospace");
    setFont(font);

    QWidget* widget = new QWidget(this);
    centralLayout_ = new QVBoxLayout(widget);
    centralLayout_->setContentsMargins(0, 0, 0, 0);
    setCentralWidget(widget);
    setStyleSheet("QMainWindow::separator{width: 4px}");
}

MenuBar* MainWindow::menuBar() {
    return dynamic_cast<MenuBar*>(QMainWindow::menuBar());
}


void MainWindow::setWorkspace(QWidget* workspace) {
    centralLayout_->addWidget(workspace);
}

void MainWindow::setSearchWidget(QWidget* searchWidget) {
    centralLayout_->addWidget(searchWidget);
}
