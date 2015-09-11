#include <QApplication>
#include <QtGui>
#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QSplitter>
#include <QTextEdit>

// TODO:
// Difference TopToBottom/BottomToTop
// Form layout

inline int show_box_layout(int argc, char **argv){
    QApplication app(argc, argv);
    QWidget wgt;

    QPushButton* btn1 = new QPushButton("A");
    QPushButton* btn2 = new QPushButton("B");
    QPushButton* btn3 = new QPushButton("C");

    QBoxLayout::Direction direct = QBoxLayout::LeftToRight;
    QBoxLayout* box = new QBoxLayout(direct);

    // Adding Widgets to Layout make them children (add to memory control)...
    box->addWidget(btn1, 1);
    box->addWidget(btn2, 2);
    box->addWidget(btn3, 3);

    // ...same with setLayout()
    wgt.setLayout(box);

    wgt.resize(450, 50);
    wgt.show();

    return app.exec();
}

inline int show_stretch(int argc, char **argv){
    QApplication app(argc, argv);
    QWidget wgt;

    QPushButton* btn1 = new QPushButton("A");
    QPushButton* btn2 = new QPushButton("B");

    QBoxLayout::Direction direct = QBoxLayout::LeftToRight;
    //QBoxLayout::Direction direct = QBoxLayout::RightToLeft;
    //QBoxLayout::Direction direct = QBoxLayout::TopToBottom;
    //QBoxLayout::Direction direct = QBoxLayout::BottomToTop;

    QBoxLayout* box = new QBoxLayout(direct);

    // Replace middle button with Stretch with factor 2
    box->addWidget(btn1, 1);
    box->addStretch(2);
    box->addWidget(btn2, 3);

    wgt.setLayout(box);

    wgt.resize(450, 50);
    wgt.show();

    return app.exec();
}

inline int show_hbox_layout(int argc, char **argv){
    QApplication app(argc, argv);
    QWidget wgt;

    QPushButton* btn1 = new QPushButton("A");
    QPushButton* btn2 = new QPushButton("B");
    QPushButton* btn3 = new QPushButton("C");

    //QHBoxLayout* box = new QHBoxLayout();
    QVBoxLayout* box = new QVBoxLayout();

    // Adding Widgets to Layout make them children (add to memory control)...
    box->addWidget(btn1);
    box->addWidget(btn2);
    box->addWidget(btn3);

    // ...same with setLayout()
    wgt.setLayout(box);

    wgt.resize(450, 50);
    wgt.show();

    return app.exec();
}

inline int show_embed_layout(int argc, char **argv){

    QApplication app(argc, argv);
    QWidget wgt;

    QPushButton* btn1 = new QPushButton("A");
    QPushButton* btn2 = new QPushButton("B");
    QPushButton* btn3 = new QPushButton("C");
    QPushButton* btn4 = new QPushButton("D");

    QHBoxLayout* hbox = new QHBoxLayout();
    QVBoxLayout* vbox = new QVBoxLayout();

    // Setting Tab order
    QWidget::setTabOrder(btn1, btn2);
    QWidget::setTabOrder(btn2, btn3);
    QWidget::setTabOrder(btn3, btn4);

    // 2 buttons in vertical
    vbox->addWidget(btn1, 1, Qt::AlignVCenter);
    vbox->addWidget(btn2, 2, Qt::AlignVCenter);
    vbox->setMargin(5);
    vbox->setSpacing(15);

    // Embed layouts
    vbox->addLayout(hbox);

    // Two buttons in horisontal
    hbox->addWidget(btn3, 1, Qt::AlignBottom);
    hbox->addWidget(btn4, 2, Qt::AlignBottom);
    hbox->setMargin(5);
    hbox->setSpacing(15);

    wgt.setLayout(vbox);

    wgt.show();

    return app.exec();
}

inline int show_grid_layout(int argc, char **argv){

    QApplication app(argc, argv);
    QWidget wgt;

    QPushButton* btn1 = new QPushButton("A");
    QPushButton* btn2 = new QPushButton("B");
    QPushButton* btn3 = new QPushButton("C");
    QPushButton* btn4 = new QPushButton("D");

    QGridLayout* gbox = new QGridLayout();
    gbox->setMargin(5);
    gbox->setSpacing(15);


    // Two buttons in horizontal
    gbox->addWidget(btn1, 0, 0);
    gbox->addWidget(btn2, 0, 1);
    gbox->addWidget(btn3, 1, 0);
    gbox->addWidget(btn4, 1, 1);

    wgt.setLayout(gbox);

    wgt.show();

    return app.exec();
}

int show_splitter(int argc, char **argv){
    QApplication app(argc, argv);
    QSplitter wgt(Qt::Vertical);
    QTextEdit* txt1 = new QTextEdit;
    QTextEdit* txt2 = new QTextEdit;
    wgt.addWidget(txt1);
    wgt.addWidget(txt2);

    txt1->setPlainText("Line1\nLine2");
    txt2->setPlainText("Line3\nLine4");

    wgt.resize(300, 200);
    wgt.show();

    return app.exec();

}

int main(int argc, char **argv)
{
    //return show_box_layout(argc, argv);
    //return show_stretch(argc, argv);
    //return show_hbox_layout(argc, argv);
    //return show_embed_layout(argc, argv);
    //return show_grid_layout(argc, argv);
    return show_splitter(argc, argv);
}