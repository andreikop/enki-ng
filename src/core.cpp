#include "search-replace/search_controller.h"

#include "core.h"


Core* Core::instance_ = nullptr;

Core::Core():
    mainWindow_(new MainWindow()),
    workspace_(new Workspace(mainWindow_))
{
    instance_ = this;
    modules_.append(new SearchController());
}

Core::~Core() {
    foreach(QObject* module, modules_) {
        delete module;
    }

    delete workspace_;
    delete mainWindow_;
}

MainWindow& Core::mainWindow() const {
    return *mainWindow_;
}

Workspace& Core::workspace() const {
    return *workspace_;
}

Core& core() {
    return *Core::instance_;
}
