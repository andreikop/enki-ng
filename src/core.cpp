#include <QDebug>

#include "workspace.h"
#include "menu_bar.h"
#include "module.h"
#include "main_window.h"
#include "project.h"
#include "settings.h"

#include "search_replace/search_controller.h"
#include "file_browser/file_browser.h"
#include "locator/locator.h"

#include "core.h"


class CoreImplementation: public Core {
public:
    void init() override {
        settings_ = std::make_unique<Settings>();
        mainWindow_ = std::make_unique<MainWindow>();
        workspace_ = std::make_unique<Workspace>(mainWindow_.get());
        project_ = std::make_unique<Project>();

        modules_.append(new SearchController());
        modules_.append(new FileBrowser());
        modules_.append(new Locator());
    }

    void cleanup() override {
        foreach(Module* module, modules_) {
            delete module;
        }
        project_.reset();
        workspace_.reset();
        mainWindow_.reset();
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

    QVector<Module*> modules_;
};

CoreImplementation* CoreImplementation::instance_ = nullptr;

Core& core() {
    // NOTE not thread safe
    if (CoreImplementation::instance_ == nullptr) {
        CoreImplementation::instance_ = new CoreImplementation();
    }

    return *CoreImplementation::instance_;
}
