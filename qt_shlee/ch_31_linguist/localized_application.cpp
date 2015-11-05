#include "localized_application.h"



LocalizedApplication::LocalizedApplication(int & argc, char ** argv) : QApplication(argc, argv)
{
    // Load system-defiled locale
    QString current_locale = QLocale::system().name();

    // Load Qt translation files
    qt_translator.load(QString(":/translations/qt_%1").arg(current_locale));

    application_translator.load(QString(":/translations/app_%1").arg(current_locale));

    // at all load our own file
    application_translator.load("main_ru.qm", ".");

    installTranslator(&qt_translator);
    installTranslator(&application_translator);
}

void LocalizedApplication::setLanguage(int index)
{
    if (1 == index){
        application_translator.load("main_en.qm", ".");
    }
    if (0 == index){
        application_translator.load("main_ru.qm", ".");
    }

    // Send LanguageChange event
    this->installTranslator(&application_translator);
    // TODO: where to reload localization???
}
