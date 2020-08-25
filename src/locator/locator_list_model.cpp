#include <cassert>

#include <QDebug>

#include "core.h"

#include "locator_list_model.h"


namespace {

double getFileScore(const QString& text, const QString& filterText) {
    if (filterText.isEmpty()) {
        return 0.;
    }

    int textCursor = text.length();

    double score = 0.;

    for (auto it = filterText.rbegin(); it != filterText.rend(); ++it) {
        QChar ch = *it;
        QStringRef textRef = text.leftRef(textCursor);
        int charIndex = textRef.lastIndexOf(ch);

        if (charIndex == -1) {
            return -1;
        } else {
            score += (text.length() - charIndex);
            textCursor = charIndex;
        }
    }

    return score;
}

/* Prepare file path to show in the listview.
NOTE This method contains some common code with getFileScore
*/
QString formatText(const QString& text, QString filterText) {
    QStringList resultParts;

    auto filterIt = filterText.rbegin();
    for (auto pathIt = text.rbegin(); pathIt != text.rend(); ++pathIt) {
        QChar pathCh = *pathIt;

        if (filterIt != filterText.rend() && (*filterIt).toLower() == pathCh.toLower()) {
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

} // anonymous namespace


LocatorListModel::LocatorListModel(const QStringList& allItems):
    allItems_(allItems)
{
    foreach(const QString& item, allItems) {
        items_.push_back(Item{0, item});
    }
}

int LocatorListModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return 0;
    } else {
        return items_.size();
    }
}

int LocatorListModel::columnCount(const QModelIndex& /*parent*/) const {
    return 1;
}

QVariant LocatorListModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole) {
        return formatText(items_[index.row()].text, commandText_);
    } else {
        return QVariant();  // TODO return file icon
    }
}

QModelIndex LocatorListModel::index(int row, int column, const QModelIndex& /*parent*/) const {
    return createIndex(row, column);
}

QModelIndex LocatorListModel::parent(const QModelIndex& /*index*/) const {
    return QModelIndex();
}

void LocatorListModel::setCommandText(const QString& cmdText) {
    commandText_ = cmdText;
    QString cmdTextLower = cmdText.toLower();
    beginResetModel();

    items_.clear();
    foreach(const QString& text, allItems_) {
        double score = getFileScore(text.toLower(), cmdTextLower);
        if (score >= 0) {
            items_.push_back(Item{score, text});
        }
    }

    std::sort(
        items_.begin(), items_.end(),
        [](const Item& a, const Item& b) -> bool {
            return a.score < b.score;
        });

    endResetModel();
}

const QString& LocatorListModel::text(const QModelIndex& index) const {
    assert(index.column() == 0);
    assert( ! index.parent().isValid());
    return items_[index.row()].text;
}
