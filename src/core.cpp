#include "search_replace/search_controller.h"
#include "file_browser/file_browser.h"

#include "core.h"


Core* Core::instance_ = nullptr;

Core::Core():
    mainWindow_(new MainWindow()),
    workspace_(new Workspace(mainWindow_)),
    project_(new Project())
{
    instance_ = this;
    modules_.append(new SearchController());
    modules_.append(new FileBrowser());
}

Core::~Core() {
    foreach(Module* module, modules_) {
        delete module;
    }

    delete project_;
    delete workspace_;
    delete mainWindow_;
}

MainWindow& Core::mainWindow() const {
    return *mainWindow_;
}

Workspace& Core::workspace() const {
    return *workspace_;
}

Project& Core::project() const {
    return *project_;
}

Core& core() {
    return *Core::instance_;
}
