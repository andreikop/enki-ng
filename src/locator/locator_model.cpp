#include <QDebug>

#include "core.h"

#include "open_file_command.h"

#include "locator_model.h"


namespace {

double getFileScore(const QString& filePath, const QString& filterText) {
    if (filterText.isEmpty()) {
        return 0.;
    }

    int textCursor = filePath.length();

    double score = 0.;

    for (auto it = filterText.rbegin(); it != filterText.rend(); ++it) {
        QChar ch = *it;
        QStringRef textRef = filePath.leftRef(textCursor);
        int charIndex = textRef.lastIndexOf(ch);

        if (charIndex == -1) {
            return -1;
        } else {
            score += (filePath.length() - charIndex);
            textCursor = charIndex;
        }
    }

    return score;
}

/* Prepare file path to show in the listview.
NOTE This method contains some common code with getFileScore
*/
QString formatFilePath(const QString& filePath, QString filterText) {
    int textCursor = filePath.length();

    QStringList resultParts;

    auto filterIt = filterText.rbegin();
    for (auto pathIt = filePath.rbegin(); pathIt != filePath.rend(); ++pathIt) {
        QChar pathCh = *pathIt;

        if (filterIt != filterText.rend() && *filterIt == pathCh) {
            resultParts << QString("<b>%1</b>").arg(pathCh);
            ++filterIt;
        } else {
            resultParts << pathCh;
        }
    }

    QString result;
    for(auto it = resultParts.rbegin(); it != resultParts.rend(); ++it) {
        result += *it;
    }

    return result;
}

}; // anonymous namespace


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
        return formatFilePath(items_[index.row()].filePath, commandText_);
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

void LocatorModel::setCommandText(const QString& text) {
    commandText_ = text;
    beginResetModel();

    items_.clear();
    foreach(const QString& filePath, core().project().fileList()) {
        double score = getFileScore(filePath, text);
        if (score >= 0) {
            items_.push_back(Item{score, filePath});
        }
    }

    std::sort(
        items_.begin(), items_.end(),
        [](const Item& a, const Item& b) -> bool {
            return a.score < b.score;
        });

    endResetModel();
}
