#include <QApplication>
#include <QFontDatabase>
#include <QDebug>

#include "settings.h"

namespace {

}  // anonymous namespace


Settings::Settings():
    storage_("Enki", "enki")
{}

QSettings& Settings::storage() {
    return storage_;
}
