#include <list>

#include <QDebug>

#include "workspace.h"
#include "menu_bar.h"
#include "module.h"
#include "main_window.h"
#include "project.h"
#include "settings.h"

#include "modules/search_replace/search_controller.h"
#include "modules/file_browser/file_browser.h"
#include "modules/locator/locator.h"
#include "modules/basic_actions/basic_actions.h"
#include "modules/recent_dirs/recent_dirs.h"

#include "core.h"


class CoreImplementation: public Core {
public:
    void init() override {
        settings_ = std::make_unique<Settings>();
        project_ = std::make_unique<Project>();
        mainWindow_ = std::make_unique<MainWindow>();
        workspace_ = std::make_unique<Workspace>(mainWindow_.get());
        mainWindow_->updateTitle();

        modules_.push_back(std::make_unique<SearchController>());
        modules_.push_back(std::make_unique<FileBrowser>());
        modules_.push_back(std::make_unique<Locator>());
        modules_.push_back(std::make_unique<BasicActions>());
        modules_.push_back(std::make_unique<RecentDirs>());
    }

    void cleanup() override {
        modules_.clear();

        workspace_.reset();
        mainWindow_.reset();
        project_.reset();
        settings_.reset();
    }

    MainWindow& mainWindow() override {
        return *mainWindow_;
    }

    Workspace& workspace() override {
        return *workspace_;
    }

    Project& project() override {
        return *project_;
    }

    Settings& settings() override {
        return *settings_;
    }

    friend Core& core();

private:

    ~CoreImplementation() {
        cleanup();
    }

    std::unique_ptr<Settings> settings_;
    std::unique_ptr<MainWindow> mainWindow_;
    std::unique_ptr<Workspace> workspace_;
    std::unique_ptr<Project> project_;

    static CoreImplementation* instance_;

    std::list<std::unique_ptr<Module>> modules_;
};

CoreImplementation* CoreImplementation::instance_ = nullptr;

Core& core() {
    // NOTE not thread safe
    if (CoreImplementation::instance_ == nullptr) {
        CoreImplementation::instance_ = new CoreImplementation();
    }

    return *CoreImplementation::instance_;
}
