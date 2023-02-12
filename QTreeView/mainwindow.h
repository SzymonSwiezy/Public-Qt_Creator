#ifndef WINDOW_H
#define WINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>
#include "filter.h"
#include <QtXml>

class QTreeView;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QTreeView *treeView_pointer;
    Filter filter;
    bool isFilterActive = false;

    void ReadXmlFile();

private slots:
    void insertChild();
    bool insertColumn();
    void deleteFunction();

    void addFile();
    void saveToFile();
    QString treeToText(const QAbstractItemModel* model, const QModelIndex& parent = QModelIndex(), const QString& head = QString());

    void ShowContextMenu(const QPoint &pos);
    void xmlReader(const QDomNode &node, QTextStream &stream, int deep);

    void showFilter();
    void on_lineEdit_textChanged(const QString &arg1);

};

#endif
