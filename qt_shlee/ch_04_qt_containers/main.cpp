#include <vector>
#include <list>
#include <algorithm>
#include <functional>

#include <QApplication>
#include <QtAlgorithms>
#include <QtCore>



void show_common(){
    QVector<int> v1;
    v1 << 1 << 2 << 3;
    QVector<int> v2;
    v2 = v1;

    // Common for every container operations
    bool b = (v1 == v2);

    qDebug() << "v1 = " << v1;
    qDebug() << "v2 = " << v2;

    int i1 = v1[0];

    QVector<int>::iterator it1 = v1.begin();
    QVector<int>::iterator it2 = v1.end();

    QVector<int>::const_iterator it3 = v1.constBegin();
    QVector<int>::const_iterator it4 = v1.constEnd();

    // make it empty
    v2.clear();

    qDebug() << "v1 = " << v1;
    qDebug() << "v2 = " << v2;

    // index, what
    v2.insert(0, i1);

    // index
    v1.remove(0);

    // index, count
    v1.remove(0, 2);

    qDebug() << "v1 = " << v1;
    qDebug() << "v2 = " << v2;

    // index
    int i2 = v2.value(0);

    // both methods are equal
    b = (v1.empty()) && (v2.isEmpty());

    // QObject could not be stored in container, pointer to it only
}

void show_iterators(){

    qDebug() << "Java-style iterators";
    QList<QString> lst;
    lst << "atatat" << "german" << "skotobaza";
    QListIterator<QString> it(lst);
    while (it.hasNext()){
        qDebug() << it.next();
    }

    // go to beginning
    it.toFront();

    // Just look next element, without moving
    qDebug() << it.peekNext();
    qDebug() << it.peekNext();

    qDebug() << "STL-style iterators (STL-algorithm compatible)";
    for (QList<QString>::const_iterator it1 = lst.begin(); it1 != lst.end(); ++it1){
        qDebug() << *it1;
    }

    // compatible with the STL
    std::for_each(lst.begin(), lst.end(), std::mem_fun_ref(&QString::clear));

    qDebug() << lst;

    // Qt foreach copies the whole container!
    // avoid using it
    Q_FOREACH(QString s, lst){
        qDebug() << s;
    }
}

void show_vector(){
    QVector<int> v1;
    v1.reserve(3);

    size_t s = v1.size();

    v1 << 1 << 2 << 3;

    // pre-sized vector, 0 by default
    QVector<double> v2(3);
    v2 << 1. << 2. << 3.;

    qDebug() << v1;
    qDebug() << v2;

    v1.push_back(4);

    // inefficient operations also supported
    // add to beginning (equal operations)
    v2.push_front(0.);
    v2.prepend(-1.);

    qDebug() << v1;
    qDebug() << v2;

    // search element, return index
    s = v2.indexOf(1.);
    s = v1.lastIndexOf(3);

    v1.resize(10); // fill by default to 10
    s = v1.size();

    qDebug() << v1;
    qDebug() << v2;

    QList<int> lst = v1.toList();
    std::vector<double> stdv1 = v2.toStdVector();

    // get C-array
    int* pv = v1.data();
    qDebug() << pv[0];
}

void show_bytes(){

    // Raw bytes array (size, fill)
    QByteArray arr(3, 0);
    qDebug() << arr;

    arr[0] = arr[1] = 0xFF;
    arr[2] = 1;

    // could be compressed/decompressed
    QByteArray txt = "Text data";
    qDebug() << txt;

    QByteArray txt_compressed = qCompress(txt);
    qDebug() << txt_compressed;

    QByteArray txt_decompressed = qUncompress(txt_compressed);
    qDebug() << txt_decompressed;

    // Could be converted to Base64 before
    QByteArray txt64 = txt.toBase64();
    qDebug() << txt64;

    // .. and back
    qDebug() << QByteArray::fromBase64(txt64);

    // array of 10 bits
    QBitArray bits(10, true);
    qDebug() << bits;

    bits.resize(20);
    qDebug() << bits;
}

void show_lists(){
    // one-direction list
    QList<int> lst1;
    lst1 << 1 << 2 << 3;

    // two-directions list
    QLinkedList<double> lst2;

    qDebug() << lst1;

    // move element
    lst1.move(0, 1);
    qDebug() << lst1;

    int i1 = lst1[0];
    int i2 = lst1.at(1);

    // transform
    qDebug() << lst1.toSet();
    qDebug() << lst1.toVector();

    std::list<double> l = lst2.toStdList();

    // QLinkedList does not have operator << for debug, operator[] and at()
}

void show_qmap(){

    // skip-list-based (!) dictionary
    QMap<int, QString> m1;

    // operator[] silently inserts like std::map
    // just replace to value() for access
    m1[0] = "zero";
    m1[1] = "one";
    m1[2] = "two";

    // default value on non-existent key
    qDebug() << m1.value(1);
    qDebug() << m1.value(5);

    qDebug() << m1;

    // methods that contained in every Qt associative container
    qDebug() << m1.contains(1);

    // QMap allows multi-index insert
    m1.insertMulti(1, "another one");
    qDebug() << m1;

    // just like multimap
    QMap<int, QString>::const_iterator it1 = m1.lowerBound(1);
    QMap<int, QString>::const_iterator it2 = m1.upperBound(1);

    qDebug() << *it1;
    qDebug() << *it2;

    // remove first 1 key
    m1.erase(m1.find(1));
    qDebug() << m1;

    qDebug() << "Keys: ";
    qDebug() << m1.keys();

    qDebug() << "Values: ";
    qDebug() << m1.values();

    // remove element and return a copy
    QString val = m1.take(0);
    qDebug() << val;

    // unite maps (duplicating keys)
    QMap<int, QString> m2 = m1;
    m2.unite(m1);
    qDebug() << m2;

    // remove doubles converting to std map
    std::map<int, QString> std_m = m2.toStdMap();

    // QMultiMap is inherited from the QMap!
    QMultiMap<int, QString> mm1(m2);
    
    qDebug() << "1 key count";
    qDebug() << mm1.count(1);

    // remove all keys
    mm1.remove(1);
    qDebug() << mm1;

    // hash-table-based set
    QSet<QString> s1;
    QSet<QString> s2;
    s1 << "one" << "two" << "three";
    s2 << "three" << "four" << "five";

    // set operations
    QSet<QString> unite_set = s1;
    unite_set.unite(s2);
    qDebug() << unite_set;

    QSet<QString> inter_set = s1;
    inter_set.intersect(s2);
    qDebug() << inter_set;

    QSet<QString> subtr_set = s1;
    subtr_set.subtract(s2);
    qDebug() << subtr_set;
}

void show_qhash(){

    // hash-table-based dictionary
    QHash<int, QString> m1;
    
    // table size could be pre-allocated
    // Ensures that the QHash's internal hash table 
    // consists of at least size buckets
    m1.reserve(10);

    qDebug() << "Capacity: ";
    qDebug() << m1.capacity();


    // operator[] silently inserts like std::map
    // just replace to value() for access
    m1[0] = "zero";
    m1[1] = "one";
    m1[2] = "two";

    // shrink by fit operation (maybe do nothing, depend on size)
    m1.squeeze();

    qDebug() << "Capacity: ";
    qDebug() << m1.capacity();


    // default value on non-existent key
    qDebug() << m1.value(1);
    qDebug() << m1.value(5);

    qDebug() << m1;
}

void show_string(){
    QString s1 = "";
    QString s2;

    qDebug() << s1.isNull();
    qDebug() << s2.isNull();

    s1 = "one";
    s2 = "two";

    qDebug() << s1.toUpper();
    qDebug() << s1;
    qDebug() << s1.toLower();
    qDebug() << s1;

    s1.setNum(10);
    qDebug() << s1;
    qDebug() << s1.toInt();

    // regexp
    QRegExp r("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}");
    QString s_ip = "This string contains IP address 192.168.1.1";
    qDebug() << (s_ip.contains(r) > 0); //???
}

void show_algorithm(){
    // Qt algorithms accept all container and overloaded

    QVector<int> v1;
    QList<int> l1;
    v1 << 1 << 2 << 3;
    l1 << 3 << 2 << 1;

    QVector<int>::const_iterator it = qBinaryFind(v1, 2);
    size_t count = 0;
    qCount(v1, 1, count);
    qDebug() << count;

    v1.append(0);
    qSort(v1);
    qSort(l1);

    qDebug() << v1;
    qDebug() << l1;

    QVector<int>::const_iterator it2 = qBinaryFind(v1, 1);
    QList<int>::const_iterator it3 = qBinaryFind(l1, 1);

    qDebug() << *it2;
    qDebug() << *it3;
}

void show_variant(){
    QVariant v1;
    QVariant v2(1);
    QVariant v3(true);
    QVariant v4("one");

    qDebug() << v1;
    qDebug() << v2;
    qDebug() << v3;
    qDebug() << v4;

    qDebug() << QVariant::typeToName(v1.type());
    qDebug() << QVariant::typeToName(v2.type());
    qDebug() << QVariant::typeToName(v3.type());
    qDebug() << QVariant::typeToName(v4.type());

    qDebug() << v1.isNull();
    qDebug() << v2.toString();
    qDebug() << v3.toInt();
    qDebug() << v4.toString();
}


int main(int argc, char **argv)
{
    QCoreApplication app (argc, argv);

    show_common();
    show_iterators();
    show_vector();
    show_lists();
    show_bytes();
    show_qmap();
    show_qhash();
    show_string();
    show_algorithm();
    show_variant();

    return app.exec();
}