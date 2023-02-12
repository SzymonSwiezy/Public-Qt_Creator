#include "dragdroplistmodel.h"
#include "qabstractitemmodel.h"
#include "qmimedata.h"
#include "qnamespace.h"
#include <QtGui>

DragDropListModel::DragDropModel(const QStringList &headers, const QString &data, QObject *parent)
    : TreeModel(headers, data, parent)
{
}


Qt::DropActions DragDropListModel::supportedDropActions() const
 {
     return Qt::CopyAction | Qt::MoveAction;
 }
Qt::ItemFlags DragDropListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = flags(index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}
QStringList DragDropListModel::mimeTypes() const
{
    QStringList types;
    types << "application/vnd.text.list";
    return types;
}
QMimeData *DragDropListModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (QModelIndex index, indexes) {
        if (index.isValid()) {
            QString text = data(index, Qt::DisplayRole).toString();
            stream << text;
        }
    }

    mimeData->setData("application/vnd.text.list", encodedData);
    return mimeData;
}
bool DragDropListModel::dropMimeData(const QMimeData *data,
    Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
        return true;

    if (!data->hasFormat("application/vnd.text.list"))
        return false;

    if (column > 0)
        return false;
    int beginRow;

      if (!parent.isValid())
          beginRow = row;
      else
          beginRow = parent.row();
      QByteArray encodedData = data->data("application/vnd.text.list");
      QDataStream stream(&encodedData, QIODevice::ReadOnly);
      QStringList newItems;
      int rows = 0;

      while (!stream.atEnd()) {
          QString text;
          stream >> text;
          newItems << text;
          ++rows;
      }
      insertRows(beginRow, rows, QModelIndex());
      foreach (QString text, newItems) {
          QModelIndex idx = index(beginRow, 0, QModelIndex());
          setData(idx, text);
          beginRow++;
      }

      return true;
  }
