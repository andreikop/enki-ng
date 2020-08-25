#pragma once

#include "core.h"

#include "settings.h"

/* Configurable by user application option.

Contains key, type and default value

Create an instance of an option in a module where you need some settings.
The Option will load it's value from global settings.
Typical usage:

namespace {
Option<QString> myStringOption("my/option_path", "Default value");
}

void MyClass::method() {
    QString optionValue = myStringOption.value();
}
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

    const T value() const {
        QSettings& storage = core().settings().storage();
        if (storage.contains(key_)) {
            return storage.value(key_).value<T>();
        } else {
            return defaultValue_;
        }
    }

    const T& defaultValue() const {
        return defaultValue_;
    }

    void setValue(const T &value) {
        QSettings& storage = core().settings().storage();
        storage.setValue(key_, value);
    }


private:
    QString key_;
    T defaultValue_;
};
