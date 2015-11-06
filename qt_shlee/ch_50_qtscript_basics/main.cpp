#include <QApplication>
#include <QScriptEngine>
#include <QScriptValue>
#include <QLabel>

// * QtSript uses such Qt facilities as signal-slot, properties, object meta-info
// * Q_OBJECT and Q_PROPERTY allows object to be used in QtScript
// * Q_INVOKABLE void scriptAccessableMethod() - method accessible to QtScript
// * Non-Qt class could be wrapped to Q_OBJECT (or private inheritance if required)

// TODO: slots - invocable?

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QLabel lbl;
    QScriptEngine scriptEngine;
    QScriptValue scriptLbl = scriptEngine.newQObject(&lbl);
    scriptEngine.globalObject().setProperty("lbl", scriptLbl);
    
    QScriptValue value = scriptEngine.evaluate("2 * 2");
    scriptEngine.evaluate("lbl.text = 'Hello, QtScript = " + value.toString() + "'");

    scriptEngine.evaluate("lbl.show()");
    return app.exec();
}