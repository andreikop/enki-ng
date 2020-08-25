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
    // Check if the core can init and cleanup multiple times
    void InitAndCleanup() {
        for(int i = 0; i < 10; i++) {
            core().init();
            core().cleanup();
        }
    }
};

QTEST_MAIN(Test)
#include "test_core.moc"
