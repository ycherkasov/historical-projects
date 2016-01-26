#include <QLabel>
#include <QComboBox>
#include <QLayout>

#include "localized_application.h"



LocalizedApplication::LocalizedApplication(int & argc, char ** argv) : 
QApplication(argc, argv),
combo_(new QComboBox),
label_(new QLabel)
{
    loadLocale();
    combo_->addItem(QObject::tr("Russian"));
    combo_->addItem(QObject::tr("English"));

    changeLanguage();

    QObject::connect(combo_,
        SIGNAL(currentIndexChanged(int)),
        this,
        SLOT(setLanguage(int)));

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(combo_);
    layout->addWidget(label_);

    app_window_.setLayout(layout);
    app_window_.resize(300, 100);
    app_window_.show();

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
    
    // reload localization
    changeLanguage();
}

void LocalizedApplication::loadLocale()
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

void LocalizedApplication::changeLanguage()
{
    // 1st param: key, 2nd: hint, 3rd: noun/plural
    label_->setText(QObject::tr("Language", "Hint for translator"));

    combo_->setItemText(0, QObject::tr("Russian"));
    combo_->setItemText(1, QObject::tr("English"));

    // also could be caught in Widget
    // if (pe->type() == QEvent::LanguageChange) ...
}
