// MySorter.cpp
#include "filter.h"

Filter::Filter(QObject *parent) : QSortFilterProxyModel(parent){
    searched_text = "";
}

bool Filter::filterAcceptsRow(int source_row, const QModelIndex & source_parent) const{
    // check the current item
    bool result = QSortFilterProxyModel::filterAcceptsRow(source_row,source_parent);

    //get the index of the item
    QModelIndex currntIndex = sourceModel()->index(source_row, filterKeyColumn(), source_parent);
    if (sourceModel()->hasChildren(currntIndex)) // keep the parent if a children is shown
    {
        for (int i = 0; i < sourceModel()->rowCount(currntIndex) && !result; ++i) result = result || filterAcceptsRow(i, currntIndex);
    }
    return result;
}

void Filter::restart(){
    searched_text = "";
}
