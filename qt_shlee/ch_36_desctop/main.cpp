#include <QApplication>
#include "screenshot_widget.h"

// * get desktop widget - QApplication::desktop()
// * Works with several desktops
// * QPixmap::grabWindow - make screenshot
// * QDesktopServices::MoviesLocation and so on - user-specific directories
// * QDesktopServices::openUrl - start default browser


int main(int argc, char** argv)
{
    QApplication app(argc, argv); 
    ScreenshotWidget wgt;
    wgt.show();
    return app.exec();
}
