#include <QApplication>
#include <QtGui>
#include <QWidget>



inline int show_box_layout(int argc, char **argv){
    QApplication app(argc, argv);

    return app.exec();
}


int main(int argc, char **argv)
{
    return show_box_layout(argc, argv);
}