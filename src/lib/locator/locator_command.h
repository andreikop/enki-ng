#pragma once

#include <QObject>
#include <QAbstractItemModel>

/**
 LocatorCommand is a generic command which is executed by Locator dialogue.
 The command provides list of choices and does the job when some item is chosen
 Concrete commands are OpenFileCommand, SwitchDirectoryCommand, ...
 */

class LocatorCommand: public QObject {
    Q_OBJECT

public:
    /** A model for locator dialogue. Provides list of choices,
     * Items text supports HTML formatting
     */
    virtual QAbstractItemModel& model() = 0;

public slots:
    /** This slot is called by the Locator dialogue when user
     * edited the command text in the input.
     * A comman might update the model
     */
    virtual void setCommandText(const QString& text) = 0;

    /** This slot is called by the Locator dialogue when user
     * activated one of the items in the model.
     * Typical behaviour is to execute the command with given parameter
     * and emit done()
     */
    virtual void onItemActivated(const QModelIndex& index) = 0;

signals:
    /// This signal shall be emitted after the command has been executed
    void done();
};
