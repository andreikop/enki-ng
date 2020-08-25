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

    void StripTrailingEmptyLines() {
        Editor* editor = core().workspace().createEmptyNotSavedFile("/tmp/enki-tests-tmp-file");
        editor->qutepart().document()->setPlainText("a\nb\nc\n\n\n");

        QCOMPARE(editor->qutepart().toPlainText(), QString("a\nb\nc\n\n\n"));
        editor->saveFile();

        QCOMPARE(editor->qutepart().toPlainText(), QString("a\nb\nc\n"));
    }

};

QTEST_MAIN(Test)
#include "test_editor.moc"
