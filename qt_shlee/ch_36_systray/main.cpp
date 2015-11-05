#include <QApplication>
#include "systray_window.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv); 

    SystrayWindow wnd;

    // tray application does not exit on widget close
    QApplication::setQuitOnLastWindowClosed(false);

    return app.exec();
}
