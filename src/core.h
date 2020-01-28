#include "workspace.h"
#include "menu_bar.h"
#include "module.h"
#include "main_window.h"
#include "project.h"


class Core {
public:
    MainWindow& mainWindow();
    Workspace& workspace();
    Project& project();

    friend Core& core();
    friend int main(int argc, char** argv);

private:
    Core();
    ~Core();

    MainWindow mainWindow_;
    Workspace workspace_;
    Project project_;

    static Core* instance_;

    QVector<Module*> modules_;
};

// Conveniensce function wich returns core instance
Core& core();
