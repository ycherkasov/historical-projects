#pragma once
#include <QApplication>
#include <QTranslator>
#include <QWidget>

class QComboBox;
class QLabel;

// Encapsulate translation in application object
class LocalizedApplication : public QApplication
{
    Q_OBJECT
public:
    LocalizedApplication(int & argc, char ** argv);

public slots:

    void setLanguage(int index);

protected:

    void loadLocale();

    void changeLanguage();

private:
    QTranslator qt_translator;
    QTranslator application_translator;

    // encapsulate widgets in the app so that translate their texts
    QWidget app_window_;
    QComboBox* combo_;
    QLabel* label_;
};

