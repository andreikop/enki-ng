#pragma once

#include <vector>

#include <QAbstractItemModel>


class OpenFileCommand;

class LocatorModel: public QAbstractItemModel {
public:
    LocatorModel(const OpenFileCommand& command);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    void setCommandText(const QString& text);
    const QString& filePath(const QModelIndex& index) const;

private:
    const OpenFileCommand& command_;

    QString commandText_;
    struct Item {
        double score;
        QString filePath;
    };

    std::vector<Item> items_;
};
