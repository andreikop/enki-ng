#include <QDebug>

#include "settings.h"

namespace {

const QString IGNORED_FILE_PATTERNS = "projects/ignored_file_patterns";
const QString IGNORED_DIRECTORY_PATTERNS = "projects/ignored_directory_patterns";
}  // anonymous namespace

Settings::Settings():
    storage_("Enki", "enki")
{}

QStringList Settings::ignoredFilePatterns() const {
    return getWithDefault<QStringList>(
        IGNORED_FILE_PATTERNS,
        defaults_.ignoredFilePatterns);
}

QStringList Settings::ignoredDirectoryPatterns() const {
    return getWithDefault<QStringList>(
        IGNORED_DIRECTORY_PATTERNS,
        defaults_.ignoredDirectoryPatterns);
}

template<class T>
T Settings::getWithDefault(const QString& key, const T& defaultVal) const {
    if (storage_.contains(key)) {
        return storage_.value(key).value<T>();
    } else {
        return defaultVal;
    }
}

Settings::Defaults::Defaults() {
    for (QString pattern: {"*.o", "*.a", "*.so", "*.pyc"}) {
        ignoredFilePatterns << pattern;
    }

    for (QString pattern: {".git", ".svn", "__pycache__"}) {
        ignoredDirectoryPatterns << pattern;
    }
}
