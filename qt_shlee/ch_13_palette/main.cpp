#include <QApplication>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QLayout>

int main(int argc, char** argv)
{
    QApplication app(argc, argv); 

    QWidget wgt;

    // Example widgets
    QSpinBox* spb = new QSpinBox;
    QPushButton* pbt = new QPushButton("New palette");
    QLabel* lbl = new QLabel("New palette");


    QPalette pal = app.palette();

    // First param could be various, see ColorRole enum
    pal.setBrush(QPalette::Button, QBrush(Qt::red, Qt::Dense3Pattern)); 
    pal.setColor(QPalette::ButtonText, Qt::blue); 
    pal.setColor(QPalette::Text, Qt::magenta); 
    pal.setColor(QPalette::Active, QPalette::Base, Qt::green);

    // better to assign palette to the whole application
    // so that all widgets looked as the same
    QApplication::setPalette(pal);

    // however, palette could be assigned to any separate widget
    //spb->setPalette(pal);

    // Set layout
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(spb);
    layout->addWidget(pbt);
    layout->addWidget(lbl);
    wgt.setLayout(layout);

    wgt.resize(100, 50); 
    wgt.show();

    return app.exec();
}
