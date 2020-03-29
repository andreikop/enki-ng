#pragma once

#include <QMainWindow>
#include <QVBoxLayout>

#include "menu_bar.h"


class MainWindow: public QMainWindow {
public:
    MainWindow();
    MenuBar* menuBar();

    void setWorkspace(QWidget* workspace);
    void setSearchWidget(QWidget* searchWidget);

public slots:
    void updateTitle();

private:
    QVBoxLayout* centralLayout_;
};

