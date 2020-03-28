#pragma once

#include <QSettings>


class Settings {
public:
    Settings();

    QSettings& storage();

private:
    QSettings storage_;
};
