#ifndef DRAGDROPLISTMODEL_H
#define DRAGDROPLISTMODEL_H


#include "treemodel.h"

class DragDropModel : public TreeModel
{
    Q_OBJECT

public:
    DragDropListModel(const QStringList &headers, const QString &data, QObject *parent = nullptr);
    Qt::DropActions supportedDropActions() const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
};

#endif // DRAGDROPLISTMODEL_H
