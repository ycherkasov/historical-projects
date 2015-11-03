#include <QApplication>
#include "settings_window.h"

// * setRestartHint() - write program into autorun in any OS

int main(int argc, char** argv)
{
    QApplication app(argc, argv); 
    SettingsWindow wnd;
    wnd.show();
    return app.exec();
}
