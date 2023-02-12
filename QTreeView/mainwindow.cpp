#include <QtGui>

#include "mainwindow.h"
#include "dragdropmodel.h"
#include "filter.h"
//#include "qtreeview.h"

#include <QXmlStreamReader>
#include <QShortcut>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTreeView>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    setupUi(this);

    //DRAG AND DROP MODE
    treeView->setDragDropMode(QAbstractItemView::InternalMove);
    treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    treeView->setDragEnabled(true);
    treeView->setAcceptDrops(true);
    treeView->setDropIndicatorShown(true);
    this->treeView_pointer  = treeView;


    //WINDOW ACTIONS CONNECTION
    connect(exitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    connect(AddFileAction, &QAction::triggered, this, &MainWindow::addFile);
    connect(SaveTreeToFileAction, &QAction::triggered, this, &MainWindow::saveToFile);
    connect(SearchAction, &QAction::triggered, this, &MainWindow::showFilter);


    //DEFAULT TREE SETUP
    QStringList headers(tr("Default tree view"));
    QString file_name("C:/Users/syzmon/Desktop/default.txt");

    QFile file(file_name);
    setWindowTitle("TreeView//"+file_name);
    file.open(QIODevice::ReadOnly);
    DragDropModel *model = new DragDropModel(headers,file.readAll(), this);
    file.close();

    filter.setSourceModel(model);
    lineEdit->setVisible(false);
    treeView->setModel(&filter);

    //CONTEXT MENU
    for (int column = 0; column < model->columnCount(); ++column) treeView->resizeColumnToContents(column);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),this, SLOT(ShowContextMenu(QPoint)));


    //SHORTCUTS
    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_N), this, SLOT(insertChild()));
    new QShortcut(QKeySequence(Qt::Key_Delete), this, SLOT(deleteFunction()));
    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_C), this, SLOT(insertColumn()));
}


void MainWindow::insertChild(){
    QModelIndex index = treeView->selectionModel()->currentIndex();
    QAbstractItemModel *model = treeView->model();

    if(!(treeView->selectionModel()->hasSelection())){ //root insert
        index = QModelIndex();
        if (!model->insertRow(0,  QModelIndex())) return;
    }
    else{ //children insert
        if (model->columnCount(index) == 0) {
            if (!model->insertColumn(0, index))
                return;
        }
        if (!model->insertRow(0, index))
            return;
        }

    for (int column = 0; column < model->columnCount(index); ++column) {
        const QModelIndex child = model->index(0, column, index);
        model->setData(child, QVariant(tr("[No data]")), Qt::EditRole);
        if (!model->headerData(column, Qt::Horizontal).isValid())
            model->setHeaderData(column, Qt::Horizontal, QVariant(tr("[No header]")), Qt::EditRole);
    }

    treeView->selectionModel()->setCurrentIndex(model->index(0, 0, index), QItemSelectionModel::ClearAndSelect);
}

void MainWindow::xmlReader(const QDomNode &node, QTextStream &stream, int deep){
    QDomNode domNode = node.firstChild();
    while(!domNode.isNull()){
        for(int i =0; i < deep; i++) stream << "\t";
        if(domNode.isElement()){
            QDomElement domElement = domNode.toElement();
            if(!domElement.isNull()){
                if(domElement.tagName() == "Child"){
                    stream<<domElement.attribute("Name", "") << "\n";
                }
            }
        }
        xmlReader(domNode, stream, deep+1);
        domNode = domNode.nextSibling();
    }
}

void MainWindow::addFile(){ // create file from selected file
    //window settings
    QString file_name = QFileDialog::getOpenFileName(this,"Open a file",QDir::homePath(), tr("Text File (*.txt) ;; XML File (*.xml)"));
    treeView->setWindowFilePath(file_name);
    QFile file(treeView->windowFilePath());
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this,"Warning", "Can not open this file");
    }
    else{
        //create and setup tree

        const QStringList headers(tr("Name"));
        setWindowTitle("TreeView//"+file_name);
        file.open(QIODevice::ReadOnly | QFile::Text);
        if(file_name.endsWith(".xml",Qt::CaseSensitive)){
            QFile file_temp("C:/Users/syzmon/Desktop/tt.txt");
            file_temp.open(QIODevice::WriteOnly | QIODevice::Text);
            QTextStream stream(&file_temp);
            QDomDocument doc;
                if(doc.setContent(&file)){
                    QDomElement elem = doc.documentElement();
                    xmlReader(elem,stream,0);
                }
                DragDropModel *model = new DragDropModel(headers,file_temp.readAll(), this);
                treeView->setModel(model);

        }
        else{
        TreeModel *model = new TreeModel(headers,file.readAll(), this);
        treeView->setModel(model);
        }
        file.close();
    }
}

QString MainWindow::treeToText(const QAbstractItemModel* model, const QModelIndex& parent, const QString& head){ //creates a string based on tree form
    QString result;
    for(int i=0, maxRow = model->rowCount(parent);i<maxRow;++i){
        const QModelIndex index = model->index(i,0,parent);
        result += head + index.data().toString() + '\n';
        if(model->hasChildren(index)) result +=treeToText(model,index,head + ' ');
        if(parent!=QModelIndex()) result += "\n";
    }
    result.chop(1);
    return result;
}

void MainWindow::saveToFile(){ //save tree to file
    QString file_name =  QFileDialog::getSaveFileName(this,"Open a file", QDir::homePath(), tr("Text File (*.txt) ;; XML File (*.xml)"));
    QFile file(file_name);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream  stream(&file);
    stream << treeToText(treeView->model(),QModelIndex(),QString());
    file.close();
}

bool MainWindow::insertColumn(){
    QAbstractItemModel *model = treeView->model();
    int column = model->columnCount();

    // Insert a column in the parent item.
    bool changed = model->insertColumn(column);
    if (changed)
        model->setHeaderData(column, Qt::Horizontal, QVariant("[No header]"), Qt::EditRole);
    return changed;
}

void MainWindow::deleteFunction(){
    const QModelIndexList rowIndexList = treeView->selectionModel()->selection().indexes();
        for(int i = 0; i < rowIndexList.count(); i++){
            QModelIndex index = rowIndexList.at(i);

            if(index==QModelIndex()) treeView->model()->removeRow(index.row(), index);
            else treeView->model()->removeRow(index.row(), index.parent() );
        }
}

//CONTECT MENU SETUP
void MainWindow::ShowContextMenu(const QPoint &pos){
   QMenu contextMenu(tr("Context menu"), this);
   QAction insertChildAction("Insert new", this), DeleteAction("Delete", this), addColumnAction("Add column", this);

   if(!(treeView->selectionModel()->hasSelection())){
       DeleteAction.setEnabled(false);
   }
   else{
       DeleteAction.setEnabled(true);
   }

   //SHORTCUTS
   insertChildAction.setShortcut(tr("CTRL+N"));
   DeleteAction.setShortcut(tr("Del"));
   addColumnAction.setShortcut(tr("CTRL+C"));


   //CONTEXT MENU ACTIONS
   connect(&insertChildAction, &QAction::triggered, this, &MainWindow::insertChild);
   contextMenu.addAction(&insertChildAction);

   connect(&DeleteAction, &QAction::triggered, this, &MainWindow::deleteFunction);
   contextMenu.addAction(&DeleteAction);

   connect(&addColumnAction, &QAction::triggered, this, &MainWindow::insertColumn);
   contextMenu.addAction(&addColumnAction);

   contextMenu.exec(mapToGlobal(pos));
}

//FILTER FUNCTION
void MainWindow::on_lineEdit_textChanged(const QString &arg1){
    filter.setFilterRegularExpression(arg1);
}

void MainWindow::showFilter(){
    if(isFilterActive) lineEdit->clear();

    isFilterActive = !isFilterActive;
    lineEdit->setVisible(isFilterActive);
}


MainWindow::~MainWindow(){
    delete treeView_pointer;
}
