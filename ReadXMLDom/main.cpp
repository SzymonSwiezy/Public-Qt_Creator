#include "mainwindow.h"
#include <QtCore>
#include <QApplication>

#include <QtXml>

void ListElements(QDomElement root, QString tagname, QString attribute){
    QDomNodeList items = root.elementsByTagName(tagname); // znajduje wszystkie elementy o jednym tagnamie
    for(int i = 0;i<items.count();i++){
        QDomNode itemnode = items.at(i);

        if(itemnode.isElement()){
            QDomElement itemele = itemnode.toElement();
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QDomDocument document;
    QFile file("C:/Users/syzmon/Desktop/MyXML.xml");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return -1;
    }
    else{
        if(document.setContent(&file)){
            return -1;
        }
        file.close();
    }
    //get root element

    QDomElement root = document.firstChildElement();

        ListElements(root,"Root", "Name");

        //get the children
        QDomNodeList childs = root.elementsByTagName("Child");
        for(int i = 0; i < childs.count();i++){
            QDomNode childsnode = childs.at(i);
            if(childsnode. isElement()){
                QDomElement child = childsnode.toElement();
                ListElements(child,"Child", "Name");
            }
        }

    return a.exec();
}
