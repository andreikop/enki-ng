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
    }

    void cleanup() {
        core().cleanup();
    }

    void DuplicatingFileNamesInOpenFileList() {
        QAbstractItemModel* model = core().mainWindow().findChild<QAbstractItemModel*>("OpenedFileModel");
        core().workspace().openFile("a/file.txt");
        QCOMPARE(model->data(model->index(0, 0)).toString(), QString("file.txt"));

        core().workspace().openFile("b/file.txt");
        QCOMPARE(model->data(model->index(0, 0)).toString(), QString("a/file.txt"));
        QCOMPARE(model->data(model->index(1, 0)).toString(), QString("b/file.txt"));

        core().workspace().openFile("a/another_file.txt");
        QCOMPARE(model->data(model->index(0, 0)).toString(), QString("another_file.txt"));
        QCOMPARE(model->data(model->index(1, 0)).toString(), QString("a/file.txt"));
        QCOMPARE(model->data(model->index(2, 0)).toString(), QString("b/file.txt"));
    }

    void OpenFileTwice() {
        Workspace& workspace = core().workspace();
        workspace.openFile("a/file.txt");
        QCOMPARE(workspace.editors().length(), 1);
        QCOMPARE(workspace.currentEditor()->qutepart().textCursorPosition().line, 0);

        workspace.openFile("a/file.txt", 2);
        QCOMPARE(workspace.editors().length(), 1);
        QVERIFY(workspace.currentEditor()->filePath().endsWith("a/file.txt"));
        QCOMPARE(workspace.currentEditor()->qutepart().textCursorPosition().line, 2);
    }

    void SortFiles() {
        Workspace& workspace = core().workspace();
        workspace.openFile("a/file.txt");

        QVERIFY(workspace.editors()[0]->filePath().endsWith("a/file.txt"));

        workspace.openFile("a/another_file.txt");
        QVERIFY(workspace.editors()[0]->filePath().endsWith("another_file.txt"));
        QVERIFY(workspace.editors()[1]->filePath().endsWith("a/file.txt"));

        workspace.openFile("b/file.txt");
        QVERIFY(workspace.editors()[0]->filePath().endsWith("another_file.txt"));
        QVERIFY(workspace.editors()[1]->filePath().endsWith("a/file.txt"));
        QVERIFY(workspace.editors()[2]->filePath().endsWith("b/file.txt"));
    }
};

QTEST_MAIN(Test)
#include "test_file_operations.moc"
