#pragma once

#include <QSettings>

class Settings {
public:
    Settings();
    QStringList ignoredFilePatterns() const;
    QStringList ignoredDirectoryPatterns() const;

    QString fontFamily() const;
    int fontSize() const;

private:

    template<class T> T getWithDefault(const QString& key, const T& defaultVal) const;

    QSettings storage_;

    struct Defaults {
        Defaults();
        QStringList ignoredFilePatterns;
        QStringList ignoredDirectoryPatterns;
        QString fontFamily;
        int fontSize;
    };

    Defaults defaults_;
};
