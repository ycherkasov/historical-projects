#include <QApplication>
#include "settings_window.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv); 
    SettingsWindow wnd;
    wnd.show();
    return app.exec();
}
