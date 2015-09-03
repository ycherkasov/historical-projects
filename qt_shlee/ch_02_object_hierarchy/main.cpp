#include <QApplication>
#include <QDebug>
#include <QList>

int main(int argc, char **argv)
{
    // event loop for the console application
    QCoreApplication app (argc, argv);

    // Objects are arranged in the hierarchy
    QObject* parent_obj = new QObject;
    parent_obj->setObjectName("Global parent");
    
    QObject* first_level1 = new QObject(parent_obj);
    QObject* first_level2 = new QObject(parent_obj);
    first_level1->setObjectName("First level object 1");
    first_level2->setObjectName("First level object 2");

    QObject* second_level1 = new QObject(first_level1);
    QObject* second_level2 = new QObject(first_level1);
    QObject* second_level3 = new QObject(first_level2);
    second_level1->setObjectName("Second level object 1");
    second_level2->setObjectName("Second level object 2");
    second_level3->setObjectName("Second level object 3");

    // Now receive metainformation from objects

    qDebug() << "1. Get all parents of some object";
    for (QObject* obj = second_level1; obj; obj = obj->parent()){
        qDebug() << obj->objectName();
    }

    qDebug() << "2. Get all children of object";
    QObjectList children = parent_obj->children();
    Q_FOREACH(QObject* item, children){
        qDebug() << item->objectName();
    }

    qDebug() << "3. Find through all children any depth of hierarchy";
    
    qDebug() << "3.1 Find only one child";
    QObject* child = parent_obj->findChild<QObject*>("Second level object 3");
    if (child){
        qDebug() << child->objectName();
    }

    qDebug() << "3.2 Find by the pattern";
    QObjectList children1 = parent_obj->findChildren<QObject*>(QRegExp("Second*"));
    Q_FOREACH(QObject* item, children1){
        qDebug() << item->objectName();
    }

    qDebug() << "3.3 Dump object info";
    parent_obj->dumpObjectInfo();

    qDebug() << "3.4 Dump object tree";
    parent_obj->dumpObjectTree();

    qDebug() << "4. Metaobject info (Qt reflection)";
    qDebug() << "Class name: " << parent_obj->metaObject()->className();
    qDebug() << "Method count: " << parent_obj->metaObject()->methodCount();

    // processing loop here
    return app.exec();
}