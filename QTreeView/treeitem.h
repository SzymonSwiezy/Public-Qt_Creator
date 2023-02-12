#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QDomNode>
#include <QVariant>

class TreeItem
{
public:
    explicit TreeItem(const QList<QVariant> &data, TreeItem *parent = nullptr);
    ~TreeItem();

    TreeItem *child(int number);
    TreeItem(const QDomNode &node, int row, TreeItem *parent = nullptr);

    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    TreeItem *parent();
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const; // row()
    bool setData(int column, const QVariant &value);
    int row() const;
//    void removeChild(int childNumber);
//    void insertChild(int pos, TreeItem *child);


    //XML
    QDomNode node() const;
private:
    QList<TreeItem*> childItems;
    QList<QVariant> itemData;
    TreeItem *parentItem;
    //XML
    QDomNode domNode;
    int rowNumber;
    //void insertChild(int pos, TreeItem *child);
};

#endif
