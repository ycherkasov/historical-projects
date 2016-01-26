#include "int_list_model.h"



IntListModel::IntListModel(QObject* parent /*= nullptr*/)
: QAbstractListModel(parent)
{
}

IntListModel::IntListModel(QList<int>& lst, QObject* parent /*= nullptr*/)
: QAbstractListModel(parent)
, list_(lst)
{
}


void IntListModel::setIntList(QList<int>& lst)
{
    list_ = lst;
}

QVariant IntListModel::data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const
{
    // return data about requested elements

    // check valid first!
    if (!index.isValid())
    {
        return QVariant();
    }

    // only if we display or edit element
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        return list_.at(index.row());
    }

    // we do not have requested data...
    return QVariant();
}

bool IntListModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{
    // write new data in model
    if (index.isValid() && role == Qt::EditRole)
    {

        // convert QVariant to the type we need (int)
        list_.replace(index.row(), value.toInt());

        // inform that the model contained data changed
        emit dataChanged(index, index);

        // success
        return true;
    }
    // some error
    return false;
}

int IntListModel::rowCount(const QModelIndex &parent /* = QModelIndex() */) const
{
    return list_.count();
}

QVariant IntListModel::headerData(int section, Qt::Orientation orientation, int role /* = Qt::DisplayRole */) const
{
    // required for tree-table headers
    if (Qt::DisplayRole != role)
    {
        return QVariant();
    }

    // for columns - sign, for rows - row number
    return (orientation == Qt::Horizontal) ? QString("Number") : QString::number(section);
}

Qt::ItemFlags IntListModel::flags(const QModelIndex &index) const
{
    if (index.isValid()){
        return (QAbstractListModel::flags(index) | Qt::ItemIsEditable);
    }
    return QAbstractListModel::flags(index);
}
