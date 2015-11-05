#include <QApplication>
#include <QLabel>
#include <QComboBox>
#include <QLayout>

#include "localized_application.h"

// TODO: where to reload language?

// 1. Prepare for localization (QObject::tr() method)
// 2. Create XML ts-files (translation source)
// lupdate.exe main.cpp -ts main_ru.ts main_en.ts
// 3. Call Linguist for large project
// linguist.exe main_ru.ts
// 4. Call lrelease for converting xml-based *.ts to binary *.qm
// lrelease.exe <ts-files list>
// 5. qm-files are loaded to QTranslator::load()



int main(int argc, char** argv)
{
    LocalizedApplication app(argc, argv);

    QWidget wgt;

    // set up layout
    QComboBox* combo = new QComboBox;

    // 1st param: key, 2nd: hint, 3rd: noun/plural
    QLabel* label = new QLabel(QObject::tr("Language", "Hint for translator"));
    combo->addItem(QObject::tr("Russian"));
    combo->addItem(QObject::tr("English"));

    QObject::connect(combo, 
        SIGNAL(currentIndexChanged(int)), 
        &app, 
        SLOT(setLanguage(int)));
    
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(combo);
    layout->addWidget(label);
    
    wgt.setLayout(layout);
    wgt.resize(300, 100);
    wgt.show();
    
    return app.exec();
}
