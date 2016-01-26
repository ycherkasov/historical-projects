#pragma once
#include <QAbstractListModel>
#include <QList>

class IntListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    
    IntListModel(QObject* parent = nullptr);

    IntListModel(QList<int>& lst, QObject* parent = nullptr);

    void setIntList(QList<int>& lst);

    // return some information about requested elements (depend on role)
    QVariant data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const;

    // write data to model
    bool setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */);

    // rows in model
    int rowCount(const QModelIndex &parent /* = QModelIndex() */) const;

    // Headers (rows and columns top strings)
    QVariant headerData(int section, Qt::Orientation orientation, int role /* = Qt::DisplayRole */) const;

    // Redefine flags so that edit values in the model
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    QList<int> list_;
};

