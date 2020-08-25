#include <QAbstractItemModel>
#include <QDebug>
#include <QObject>
#include <QTest>

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

    void WindowTitle() {
        MainWindow& mainWin = core().mainWindow();
        QCOMPARE(mainWin.windowTitle(), QDir::current().path());

        core().workspace().openFile("a/file.txt");
        QCOMPARE(mainWin.windowTitle(), QString("test-project - a/file.txt[*]"));

    }
};

QTEST_MAIN(Test)
#include "test_main_window.moc"
