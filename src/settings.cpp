#include <QApplication>
#include <QFontDatabase>
#include <QDebug>

#include "settings.h"

namespace {

const QString IGNORED_FILE_PATTERNS = "projects/ignored_file_patterns";
const QString IGNORED_DIRECTORY_PATTERNS = "projects/ignored_directory_patterns";

const QString FONT_FAMILY = "editor/font_family";
const QString FONT_SIZE = "editor/font_size";
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

QString Settings::fontFamily() const {
    return getWithDefault<QString>(
        FONT_FAMILY,
        defaults_.fontFamily);
}

int Settings::fontSize() const {
    return getWithDefault<int>(
        FONT_SIZE,
        defaults_.fontSize);
}

template<class T>
T Settings::getWithDefault(const QString& key, const T& defaultVal) const {
    if (storage_.contains(key)) {
        return storage_.value(key).value<T>();
    } else {
        return defaultVal;
    }
}

namespace {

// Default font family, which depend on platform
QString defaultFontFamily() {
    /*
    Monaco - old Mac font,
    Menlo - modern Mac font,
    Consolas - default font for Windows Vista+
    Lucida Console - on Windows XP
    Monospace - default for other platforms
    */

    QStringList fontFamilies = {"Menlo", "Monaco", "Monospace", "Consolas", "Lucida Console"};
    QStringList availableFontFamilies = QFontDatabase().families();
    for(const auto& fontFamily: fontFamilies) {
        if (availableFontFamilies.contains(fontFamily)) {
            return fontFamily;
        }
    }

    return "Monospace";
}
}  // anonymous namespace

Settings::Defaults::Defaults() {
    for (QString pattern: {"*.o", "*.a", "*.so", "*.pyc"}) {
        ignoredFilePatterns << pattern;
    }

    for (QString pattern: {".git", ".svn", "__pycache__"}) {
        ignoredDirectoryPatterns << pattern;
    }

    fontFamily = defaultFontFamily();
    fontSize = QApplication::font().pointSize();
}
