#pragma once
#include <QAbstractTableModel>
#include <QHash>
#include <QVariant>

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    
    TableModel(int rows, int columns, QObject* parent = 0);

    // return some information about requested elements (depend on role)
    QVariant data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const;

    // write data to model
    bool setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */);

    // rows in model
    int rowCount(const QModelIndex &parent /* = QModelIndex() */) const;

    // columns in model
    int columnCount(const QModelIndex &parent /* = QModelIndex() */) const;

    // Headers (rows and columns top strings)
    QVariant headerData(int section, Qt::Orientation orientation, int role /* = Qt::DisplayRole */) const;

    // Redefine flags so that edit values in the model
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    
    int rows_;
    int cols_;

    // Really model data is saved in the hash table
    QHash<QModelIndex, QVariant> data_;
};

