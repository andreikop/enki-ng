#pragma once

#include <QSettings>

/* Internal option representation.
Contains key, type and default value
*/
template <class T>
class Option {
public:
    Option(const QString& key, const T& defaultValue):
        key_(key),
        defaultValue_(defaultValue)
    {}

    const QString& key() const {
        return key_;
    }

    const T& defaultValue() const {
        return defaultValue_;
    }

private:
    QString key_;
    T defaultValue_;
};

class Settings {
public:
    Settings();
    QStringList ignoredFilePatterns() const;
    QStringList ignoredDirectoryPatterns() const;

    QString fontFamily() const;
    int fontSize() const;

private:

    template<class T> T getWithDefault(const QString& key, const T& defaultVal) const;
    template<class T> T getOptionValue(const Option<T>& option) const;

    QSettings storage_;

    Option<int> fontSize_;
    Option<QString> fontFamily_;
    Option<QStringList> ignoredFilePatterns_;
    Option<QStringList> ignoredDirectoryPatterns_;
};
