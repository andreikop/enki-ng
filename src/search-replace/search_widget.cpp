#include "core.h"
#include "search_widget.h"

SearchWidget::SearchWidget():
    QFrame(core()->mainWindow()) {
    setupUi(this);
}

void SearchWidget::setMode(int mode) {
    show();
}
