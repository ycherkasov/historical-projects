#pragma once
#include <QObject>


class Test_ExampleClass : public QObject
{
    Q_OBJECT
public slots:
    void testMin();
    void testMax();
};

