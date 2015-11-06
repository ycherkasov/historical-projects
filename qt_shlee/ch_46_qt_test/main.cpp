#include <QtTest\QtTest>
#include "test_example_class.h"

// TODO: does not work...

#if 0
QTEST_MAIN(Test_ExampleClass)
#else
QT_BEGIN_NAMESPACE
QTEST_ADD_GPU_BLACKLIST_SUPPORT_DEFS
QT_END_NAMESPACE

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    QTEST_DISABLE_KEYPAD_NAVIGATION
    QTEST_ADD_GPU_BLACKLIST_SUPPORT
    Test_ExampleClass tc;
    QTEST_SET_MAIN_SOURCE_PATH
    return QTest::qExec(&tc, argc, argv);
}
#endif