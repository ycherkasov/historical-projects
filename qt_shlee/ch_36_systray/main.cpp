#include <QApplication>
#include "systray_window.h"

// TODO: 

// * QSystemTrayIcon is responsible for application try
// * Usage: setIcon()/show()/showMessage()/setToolTip()
// * Set setQuitOnLastWindowClosed(false) and catch closeEvent() so that hide instead
// * QMenu::addAction and QSystemTrayIcon::setContextMenu() - to create a tray menu

int main(int argc, char** argv)
{
    QApplication app(argc, argv); 

    SystrayWindow wnd;

    // tray application does not exit on widget close
    QApplication::setQuitOnLastWindowClosed(false);

    return app.exec();
}
