#ifndef DRAGDROPMODEL_H
#define DRAGDROPMODEL_H

#include "treemodel.h"

class DragDropModel : public TreeModel
{
    Q_OBJECT

public:
    DragDropModel(const QStringList &headers, const QString &data, QObject *parent = nullptr);

    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    QStringList mimeTypes() const override;
    Qt::DropActions supportedDropActions() const override;
};

#endif
