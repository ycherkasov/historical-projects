#include <QApplication>
#include <QtGui>
#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QSplitter>
#include <QTextEdit>
#include <QTabWidget>


// TODO:
// Difference TopToBottom/BottomToTop
// Form layout

QWidget* show_box_layout(QWidget* parent){
    
    QWidget* wgt = new QWidget(parent);

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
    wgt->setLayout(box);
    return wgt;
}

QWidget* show_stretch(QWidget* parent){
    
    QWidget* wgt = new QWidget(parent);

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

    wgt->setLayout(box);
    return wgt;
}

QWidget* show_hbox_layout(QWidget* parent){
    
    QWidget* wgt = new QWidget(parent);

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
    wgt->setLayout(box);
    return wgt;
}

QWidget* show_embed_layout(QWidget* parent){

    QWidget* wgt = new QWidget(parent);

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

    wgt->setLayout(vbox);

    return wgt;
}

QWidget* show_grid_layout(QWidget* parent){

    QWidget* wgt = new QWidget(parent);

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

    wgt->setLayout(gbox);
    return wgt;
}

QWidget* show_splitter(QWidget* parent){

    QSplitter* wgt = new QSplitter(parent);

    QTextEdit* txt1 = new QTextEdit;
    QTextEdit* txt2 = new QTextEdit;
    wgt->addWidget(txt1);
    wgt->addWidget(txt2);

    txt1->setPlainText("Line1\nLine2");
    txt2->setPlainText("Line3\nLine4");

    return wgt;
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QTabWidget wnd;
    wnd.addTab(show_box_layout(&wnd), "Box layout");
    wnd.addTab(show_stretch(&wnd), "Stretch");
    wnd.addTab(show_hbox_layout(&wnd), "HBox layout");
    wnd.addTab(show_embed_layout(&wnd), "Embed layouts");
    wnd.addTab(show_grid_layout(&wnd), "Grid layout");
    wnd.addTab(show_splitter(&wnd), "Splitter");

    wnd.resize(400, 200);
    wnd.show();

    return app.exec();

}