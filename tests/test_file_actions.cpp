#include <QAbstractItemModel>
#include <QDebug>
#include <QObject>
#include <QTest>
#include <QTimer>
#include <QDialog>

#include "core/core.h"
#include "core/workspace.h"
#include "core/project.h"


class Test: public QObject {
    Q_OBJECT

private slots:
    void init() {
        core().init();
    }

    void cleanup() {
        core().cleanup();
    }

    void NewFile() {
        QCOMPARE(core().workspace().editors().size(), 0);
        core().mainWindow().menuBar()->fileNewAction()->trigger();
        QCOMPARE(core().workspace().editors().size(), 1);

        Editor* editor = core().workspace().currentEditor();
        QCOMPARE(editor->filePath(), QString::null);

        QVERIFY(core().mainWindow().windowTitle().endsWith(" - New file[*]"));
        // TODO Save not tested. Can not simulate interaction with modal dialogue
    }
};

QTEST_MAIN(Test)
#include "test_file_actions.moc"
