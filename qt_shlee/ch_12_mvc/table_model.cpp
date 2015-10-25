#include "table_model.h"



TableModel::TableModel(int rows, int columns, QObject* parent /*= 0*/)
: QAbstractTableModel(parent),
rows_(rows),
cols_(columns)
{
}

QVariant TableModel::data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    QString ret = QString("%1.%2").arg(index.row()).arg(index.column());

    // only if we display or edit element
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        return data_.value(index, QVariant(ret));
    }

    // we do not have requested data...
    return QVariant();

}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{
    // write new data in model
    if (index.isValid() && role == Qt::EditRole)
    {
        // write data
        data_[index] = value;

        // inform that the model contained data changed
        emit dataChanged(index, index);

        // success
        return true;
    }
    return false;
}

int TableModel::rowCount(const QModelIndex &parent /* = QModelIndex() */) const
{
    return rows_;
}

int TableModel::columnCount(const QModelIndex &parent /* = QModelIndex() */) const
{
    return cols_;
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role /* = Qt::DisplayRole */) const
{
    // required for tree-table headers
    if (Qt::DisplayRole != role)
    {
        return QVariant();
    }

    return QString::number(section);
}


Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (index.isValid()){
        return (QAbstractTableModel::flags(index) | Qt::ItemIsEditable);
    }
    return QAbstractTableModel::flags(index);
}
