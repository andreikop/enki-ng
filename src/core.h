#include "workspace.h"
#include "menu_bar.h"
#include "main_window.h"


class Core {
public:
    Workspace* workspace() const;
    MainWindow* mainWindow() const;

    friend Core* core();
    friend int main(int argc, char** argv);

private:
    Core();
    ~Core();

    MainWindow* mainWindow_;
    Workspace* workspace_;

    static Core* instance_;

    QVector<QObject*> modules_;
};

// Conveniensce function wich returns core instance
Core* core();
