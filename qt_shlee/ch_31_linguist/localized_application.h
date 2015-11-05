#pragma once
#include <QApplication>
#include <QTranslator>

// Encapsulate translation in application object
class LocalizedApplication : public QApplication
{
    Q_OBJECT
public:
    LocalizedApplication(int & argc, char ** argv);

public slots:
    void setLanguage(int index);

private:
    QTranslator qt_translator;
    QTranslator application_translator;
};

