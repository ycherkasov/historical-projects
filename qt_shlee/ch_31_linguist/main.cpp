#include <QApplication>

#include "localized_application.h"

// TODO: maybe catch QEvent::LanguageChange in custom Widget?

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
    return app.exec();
}
