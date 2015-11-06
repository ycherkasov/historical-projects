#include <QtTest\QtTest>

#include "example_class.h"
#include "test_example.h"



void Test_ExampleClass::testMin()
{
    ExampleClass ex;
    QCOMPARE(ex.min(25, 0), 0);
    QCOMPARE(ex.min(-12, -5), -12);
    QCOMPARE(ex.min(2007, 2007), 2007);
    QCOMPARE(ex.min(-12, 5), -12);
}

void Test_ExampleClass::testMax()
{
    ExampleClass ex;
    QCOMPARE(ex.max(25, 0), 25);
    QCOMPARE(ex.max(-12, -5), -5);
    QCOMPARE(ex.max(2007, 2007), 2007);
    QCOMPARE(ex.max(-12, 5), 5);
}
