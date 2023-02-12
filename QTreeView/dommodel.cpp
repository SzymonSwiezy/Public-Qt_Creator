
#include "dommodel.h"
#include "treeitem.h"

#include <QtXml>

//! [0]
DomModel::DomModel(const QDomDocument &document, QObject *parent)
    : QAbstractItemModel(parent),
      domDocument(document),
      rootItem(new TreeItem(domDocument, 0))
{
}
//! [0]

//! [1]
DomModel::~DomModel()
{
    delete rootItem;
}
//! [1]

//! [2]
int DomModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}
//! [2]

//! [3]
QVariant DomModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = getItem(index);

    const QDomNode node = item->node();
//! [3] //! [4]

    switch (index.column()) {
        case 0:
            return node.nodeName();
        default:
            break;
    }
    return QVariant();
}
//! [4]

//! [5]
Qt::ItemFlags DomModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}
//! [5]

//! [6]
QVariant DomModel::headerData(int section, Qt::Orientation orientation,
                              int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0:
                return tr("Name");
      }
    }
    return QVariant();
}
//! [6]

//! [7]
QModelIndex DomModel::index(int row, int column, const QModelIndex &parent) const //ok
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    DomItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<DomItem*>(parent.internalPointer());
//! [7]

//! [8]
    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}
//! [8]

//! [9]
QModelIndex DomModel::parent(const QModelIndex &child) const //ok
{
    if (!child.isValid())
        return QModelIndex();

    DomItem *childItem = static_cast<DomItem*>(.internalPointer());
    DomItem *parentItem = childItem->parent();

    if (!parentItem || parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}
//! [9]

//! [10]
int DomModel::rowCount(const QModelIndex &parent) const //ok
{
    if (parent.column() > 0)
        return 0;

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        TreeItem *parentItem = getItem(parent);

    return parentItem->node().childNodes().count();
}
//! [10]
