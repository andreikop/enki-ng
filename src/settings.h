#pragma once

#include <QSettings>

class Settings {
public:
    Settings();
    QStringList ignoredFilePatterns() const;
    QStringList ignoredDirectoryPatterns() const;

private:

    template<class T> T getWithDefault(const QString& key, const T& defaultVal) const;

    QSettings storage_;

    struct Defaults {
        Defaults();
        QStringList ignoredFilePatterns;
        QStringList ignoredDirectoryPatterns;
    };

    Defaults defaults_;
};
