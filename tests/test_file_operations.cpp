#include <QAbstractItemModel>
#include <QDebug>
#include <QObject>
#include <QTest>

#include "core.h"
#include "workspace.h"
#include "project.h"


class Test: public QObject {
    Q_OBJECT

private slots:
    void init() {
        core().init();
        // assume the tests are executed from enki-ng/build directory
        core().project().setPath(QDir("test-project"));
    }

    void cleanup() {
        core().cleanup();
    }

    void DuplicatingFileNames() {
        QAbstractItemModel* model = core().mainWindow().findChild<QAbstractItemModel*>("OpenedFileModel");
        core().workspace().openFile("a/file.txt");
        QCOMPARE(model->data(model->index(0, 0)).toString(), QString("file.txt"));

        core().workspace().openFile("b/file.txt");
        QCOMPARE(model->data(model->index(0, 0)).toString(), QString("a/file.txt"));
        QCOMPARE(model->data(model->index(1, 0)).toString(), QString("b/file.txt"));

        core().workspace().openFile("a/another_file.txt");
        QCOMPARE(model->data(model->index(0, 0)).toString(), QString("a/file.txt"));
        QCOMPARE(model->data(model->index(1, 0)).toString(), QString("b/file.txt"));
        QCOMPARE(model->data(model->index(2, 0)).toString(), QString("another_file.txt"));
    }
};

QTEST_MAIN(Test)
#include "test_file_operations.moc"
