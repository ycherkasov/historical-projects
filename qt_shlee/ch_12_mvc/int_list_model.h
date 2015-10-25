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

    QVariant data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */);

    int rowCount(const QModelIndex &parent /* = QModelIndex() */) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role /* = Qt::DisplayRole */) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    QList<int> list_;
};

