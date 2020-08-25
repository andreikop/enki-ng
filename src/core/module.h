#pragma once

#include <QObject>

/* Module is an object which adds some functionality
to Enki.
The majority of extensible editors call such an entities a Plugin.
But Enki modules are statically linked to the core.

Module uses Core API but usually doesn't provide any APIs for
the core and other modules.

This class doesn't provide any functionality.
Modules are inherited from it rather to clearly mark a entrypoint class.

Modules are created after core initialization and deleted before core termination.
*/
class Module: public QObject {
};