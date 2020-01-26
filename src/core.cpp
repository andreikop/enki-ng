#include "search_replace/search_controller.h"
#include "file_browser/file_browser.h"

#include "core.h"


Core* Core::instance_ = nullptr;

Core::Core():
    mainWindow_(new MainWindow()),
    workspace_(new Workspace(mainWindow_))
{
    instance_ = this;
    modules_.append(new SearchController());
    modules_.append(new FileBrowserDock(mainWindow_));
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
