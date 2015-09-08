#include <ctime>
#include <QApplication>
#include <QtGui>
#include <QWidget>
#include <QStackedWidget>
#include <QScrollArea>

// TODO: 
// QApplication::setOverrideCursor()
// QStackedWidget


// Setting background of widget
inline int show_widget_background(int argc, char **argv){

    QApplication app(argc, argv);
    // Top-level widget
    QWidget wgt;

    QWidget* wgt1 = new QWidget(&wgt);
    QPalette pal1;
    pal1.setColor(wgt1->backgroundRole(), Qt::blue);
    wgt1->move(25, 25);
    wgt1->resize(100, 100);
    wgt1->setPalette(pal1);
    // draw all children background before paint
    wgt1->setAutoFillBackground(true);

    QWidget* wgt2 = new QWidget(&wgt);
    QPalette pal2;
    QPixmap pix("camera80x80.jpeg");
    QBrush br(pix);
    pal2.setBrush(wgt2->backgroundRole(), br);
    wgt2->move(75, 75);
    wgt2->resize(80, 80);
    wgt2->setPalette(pal2);
    // draw all children background before paint
    wgt2->setAutoFillBackground(true);

    wgt.resize(200, 200);
    wgt.show();

    // processing loop here
    return app.exec();
}

inline int show_mouse_pointers(int argc, char **argv){
    QApplication app(argc, argv);
    QWidget wgt;
    
    // use random cursor pointer every time
    qsrand(time(NULL));
    Qt::CursorShape cur_shape = static_cast<Qt::CursorShape>(qrand() % 21);

    QCursor c(cur_shape);
    wgt.setCursor(c);
    wgt.resize(200, 100);

    wgt.show();

    return app.exec();
}


inline int show_frames(int argc, char **argv){
    QApplication app(argc, argv);
    QFrame frm;
    frm.setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    
    frm.resize(200, 100);
    frm.show();
    return app.exec();
}


inline int show_scroll(int argc, char **argv){
    QApplication app(argc, argv);
    QWidget* wgt = new QWidget;
    QScrollArea sa;

    QPixmap pix("manule.jpg");
    QPalette pal;
    pal.setBrush(wgt->backgroundRole(), QBrush(pix));

    wgt->setPalette(pal);
    wgt->setAutoFillBackground(true);
    wgt->setFixedSize(pix.width(), pix.height());

    // QScrollArea is a visible window. Widget is a "full-size" partially visible
    sa.setWidget(wgt);
    sa.resize(300, 150);
    sa.show();

    return app.exec();
}


int main(int argc, char **argv)
{
    //return show_widget_background(argc, argv);
    //return show_mouse_pointers(argc, argv);
    //return show_frames(argc, argv);
    return show_scroll(argc, argv);
}