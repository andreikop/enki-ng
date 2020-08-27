#pragma once

#include <QMainWindow>

#include "core/option.h"
#include "qutepart/qutepart.h"


// File editor. Contains editor widget(Qutepart) instance and related information
class Editor {
public:
    Editor(const QString& filePath, const QString& text, QMainWindow* parent);

    const QString& filePath() const;
    void setFilePath(const QString& filePath);

    Qutepart::Qutepart& qutepart();

    void saveFile();

    static Option<int>& getFontSizeOption();

private:
    QString filePath_;
    Qutepart::Qutepart qutepart_;
    QString lineSeparator;

    QString textForSaving() const;

    void stripTrailingWhitespace();
    void stripTrailingEmptyLines();
};
