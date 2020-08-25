#pragma once

#include <vector>

#include <QAbstractItemModel>
#include <QStringList>


/**
This is a model which represents a list of items one of which
can be choosen in Locator dialogue with fuzzy search.
Use cases are:
    - Choose file from project when opening
    - Choose one of the items from the list of recent projects
*/
class LocatorListModel: public QAbstractItemModel {
public:
    LocatorListModel(const QStringList& allItems);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    void setCommandText(const QString& text);
    const QString& text(const QModelIndex& index) const;

private:
    QStringList allItems_;
    QString commandText_;
    struct Item {
        double score;
        QString text;
    };

    std::vector<Item> items_;
};
