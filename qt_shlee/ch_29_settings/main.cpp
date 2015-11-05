#include <QApplication>
#include "settings_window.h"


// * Settings are saved in cross-platform way
// (Windows registry, txt file)
// * Use QSettings object
// * QSettings constructor accepts company and app name (for the Registry)
// * setRestartHint() - write program into autorun in any OS
// * QSettings could be embed as a member both to App-inherited ow Windeg-inherited class

int main(int argc, char** argv)
{
    QApplication app(argc, argv); 
    SettingsWindow wnd;
    wnd.show();
    return app.exec();
}
