#pragma once
#include <QAbstractTableModel>
#include <QHash>
#include <QVariant>

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    
    TableModel(int rows, int columns, QObject* parent = 0);

    QVariant data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */);

    int rowCount(const QModelIndex &parent /* = QModelIndex() */) const;

    int columnCount(const QModelIndex &parent /* = QModelIndex() */) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role /* = Qt::DisplayRole */) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    int rows_;
    int cols_;
    QHash<QModelIndex, QVariant> data_;
};

