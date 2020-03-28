#pragma once

#include "core.h"

#include "settings.h"

/* Internal option representation.
Contains key, type and default value

Create an instance of an option in a module where you need some settings.
The Option will load it's value from global settings.
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

private:
    QString key_;
    T defaultValue_;
};
