#include <QApplication>
#include <QFontDatabase>
#include <QDebug>

#include "settings.h"

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


Settings::Settings():
    storage_("Enki", "enki"),
    fontSize_("editor/font_size", QApplication::font().pointSize()),
    fontFamily_("editor/font_family", defaultFontFamily()),
    ignoredFilePatterns_(
        "projects/ignored_file_patterns",
        {"*.o", "*.a", "*.so", "*.pyc"}),
    ignoredDirectoryPatterns_(
        "projects/ignored_directory_patterns",
        {".git", ".svn", "__pycache__"})
{}

QStringList Settings::ignoredFilePatterns() const {
    return getOptionValue<QStringList>(ignoredFilePatterns_);
}

QStringList Settings::ignoredDirectoryPatterns() const {
    return getOptionValue<QStringList>(ignoredDirectoryPatterns_);
}

QString Settings::fontFamily() const {
    return getOptionValue<QString>(fontFamily_);
}

int Settings::fontSize() const {
    return getOptionValue<int>(fontSize_);
}

template<class T>
T Settings::getOptionValue(const Option<T>& option) const {
    const QString& key = option.key();

    if (storage_.contains(key)) {
        return storage_.value(key).value<T>();
    } else {
        return option.defaultValue();
    }
}
