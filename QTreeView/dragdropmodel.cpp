#include <QtGui>

#include "dragdropmodel.h"
DragDropModel::DragDropModel(const QStringList &headers, const QString &data, QObject *parent)
    : TreeModel(headers, data, parent)
{
}

QMimeData *DragDropModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (QModelIndex index, indexes){
        if(index.isValid()){
            QString text = data(index, Qt::DisplayRole | Qt::EditRole).toString();
            stream << index.internalId() << index.row() << index.column() << text;
        }
    }

    mimeData->setData("application/vnd.text.list", encodedData);
    return mimeData;
}

QStringList DragDropModel::mimeTypes() const
{
    QStringList types;
    types << "application/vnd.text.list";
    return types;
}

Qt::DropActions DragDropModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

bool DragDropModel::dropMimeData(const QMimeData *data,
    Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
        return true;

    if (!data->hasFormat("application/vnd.text.list"))
        return false;

    int beginRow;

    if (row != -1)
        beginRow = row;
    else if (parent.isValid())
        beginRow = 0;
    else
        beginRow = rowCount(QModelIndex());

    QByteArray encodedData = data->data("application/vnd.text.list");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QHash<qint64, QMap<int,QHash<int,QString> > > newItems;

    while (!stream.atEnd()) {
        qint64 id;
        int row;
        QString text;
        stream >> id >> row >> column >> text;
        newItems[id][row][column] = text;
    }
    int rows = newItems.count();

    insertRows(beginRow, rows, parent);
    QMap<int,QHash<int,QString> > childItems;
    foreach (childItems, newItems.values()) {
        QHash<int,QString> rowItems;
        foreach (rowItems, childItems.values()) {
            foreach (int column, rowItems.keys()) {
                QModelIndex idx = index(beginRow, column, parent);
                setData(idx, rowItems[column]);
            }
            ++beginRow;
        }
    }
    return true;
}
