#pragma once

class Settings;
class MainWindow;
class Workspace;
class Project;

class Core {
public:
    virtual MainWindow& mainWindow() = 0;
    virtual Workspace& workspace() = 0;
    virtual Project& project() = 0;
    virtual Settings& settings() = 0;

protected:
    virtual ~Core() = default;
};

// Conveniensce function wich returns core instance
Core& core();
