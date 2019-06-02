#pragma once

#include <QMainWindow>

#include "qutepart.h"


// File editor. Contains editor widget(Qutepart) instance and related information
class Editor {
public:
    Editor(const QString& filePath, const QString& text, QMainWindow* parent);

    const QString& filePath() const;
    Qutepart::Qutepart& qutepart();

    void saveFile();

private:
    QString m_filePath;
    Qutepart::Qutepart m_qutepart;
};

