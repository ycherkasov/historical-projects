#include <QApplication>
#include "screenshot_widget.h"

// TODO:
// QDesktopServices::MoviesLocation
// QDesktopServices::openUrl

int main(int argc, char** argv)
{
    QApplication app(argc, argv); 
    ScreenshotWidget wgt;
    wgt.show();
    return app.exec();
}
