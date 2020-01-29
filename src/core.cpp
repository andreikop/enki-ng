#include "search_replace/search_controller.h"
#include "file_browser/file_browser.h"
#include "locator/locator.h"

#include "core.h"


Core* Core::instance_ = nullptr;

Core::Core():
    workspace_(&mainWindow_)
{
    instance_ = this;
    modules_.append(new SearchController());
    modules_.append(new FileBrowser());
    modules_.append(new Locator());
}

Core::~Core() {
    foreach(Module* module, modules_) {
        delete module;
    }
}

MainWindow& Core::mainWindow() {
    return mainWindow_;
}

Workspace& Core::workspace() {
    return workspace_;
}

Project& Core::project() {
    return project_;
}

Core& core() {
    return *Core::instance_;
}
