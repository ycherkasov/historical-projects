#include <QApplication>
#include <QScriptEngine>
#include <QScriptValue>
#include <QLabel>

// * QtSript uses such Qt facilities as signal-slot, properties, object meta-info
// * Q_OBJECT and Q_PROPERTY allows object to be used in QtScript
// * Q_INVOKABLE void scriptAccessableMethod() - method accessible to QtScript
// * Non-Qt class could be wrapped to Q_OBJECT (or private inheritance if required)
// * The module is deprecated as of Qt 5.5

/*
People wanted to update Qt Script from JavaScriptCore to V8. 
They wanted Qt Script to provide JavaScript for Qt WebKit, QML, and user-scripting.

However, they found out that it is not feasible to update Qt Script like this. 
This is because Qt Script has many design problems 
(see http://article.gmane.org/gmane.comp.lib.qt.devel/4224 ). 
So, they created a new JavaScript engine for QML (QJSEngine).
*/

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