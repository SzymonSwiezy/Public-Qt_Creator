#ifndef FILTER_H
#define FILTER_H

#include <QSortFilterProxyModel>
#include <QObject>

class Filter: public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit Filter(QObject *parent = nullptr);
    void restart();

private:
    QString searched_text;

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex & source_parent) const override; // czy wiersz jest dostępny
};

#endif // FILTER_H
