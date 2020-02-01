#include <QDebug>

#include "core.h"

#include "open_file_command.h"

#include "locator_model.h"


LocatorModel::LocatorModel(const OpenFileCommand& command):
    command_(command)
{
    foreach(const QString& filePath, core().project().fileList()) {
        items_.push_back(Item{0, filePath});
    }
}

int LocatorModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return 0;
    } else {
        return items_.size();
    }
}

int LocatorModel::columnCount(const QModelIndex &parent) const {
    return 1;
}

QVariant LocatorModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole) {
        return items_[index.row()].filePath;
    } else {
        return QVariant();  // TODO return file icon
    }
}

QModelIndex LocatorModel::index(int row, int column, const QModelIndex &parent) const {
    return createIndex(row, column);
}

QModelIndex LocatorModel::parent(const QModelIndex &index) const {
    return QModelIndex();
}

namespace {

double getFileScore(const QString& filePath, const QString& filterText) {
    foreach(QChar ch, filterText) {
        if ( ! filePath.contains(ch)) {
            return 0.;
        }
    }
    return 1.;
}

}; // anonymous namespace

void LocatorModel::setCommandText(const QString& text) {
    beginResetModel();

    items_.clear();
    foreach(const QString& filePath, core().project().fileList()) {
        double score = getFileScore(filePath, text);
        if (score > 0) {
            items_.push_back(Item{score, filePath});
        }
    }

    std::sort(
        items_.begin(), items_.end(),
        [](const Item& a, const Item& b) -> bool {
            return a.score > b.score;
        });

    endResetModel();
}
