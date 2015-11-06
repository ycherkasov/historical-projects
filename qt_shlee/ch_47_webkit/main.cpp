#include <QApplication>
#include "web_browser.h"

// TODO: It does not work as the Web kit does not installed. Do it later!

int main(int argc, char** argv)
{
    QApplication app(argc, argv); 
    
    WebBrowser wnd;
    wnd.show();

    return app.exec();
}
