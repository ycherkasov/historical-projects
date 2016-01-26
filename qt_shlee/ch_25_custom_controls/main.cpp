#include <QApplication>
#include <QTabWidget>
#include <QLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QString>
#include <QScrollBar>

#include "hex_line_edit.h"
#include "custom_progress_bar.h"

// * Create your own widget so that re-implement draw, input or any other aspect
// * Methods that could be re-implemented:
// paintEvent, resizeEvent, mouse etc...
// * Also for some specific styles, input checks etc (see HexLineEdit)
// * While design decide is that class could be a parent (protected section)
// * Decide what signals and slots support
// * Layout behavior - implement virtual QSize sizeHint() and set size policy
// (see enum SizeType)

QWidget* get_hex_edit(QWidget* parent){

    QWidget* wgt = new QWidget(parent);

    // these edits support only Hex format
    HexLineEdit* hex_edit1 = new HexLineEdit;
    HexLineEdit* hex_edit2 = new HexLineEdit;

    // Set up layout
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(hex_edit1);
    layout->addWidget(hex_edit2);
    wgt->setLayout(layout);

    return wgt;
}

QWidget* get_size_hints(QWidget* parent){

    QWidget* wgt = new QWidget(parent);

    // Buttons that should have different size hints
    QPushButton* btn1 = new QPushButton("Short label");
    QPushButton* btn2 = new QPushButton("Very-very long label with long text");

    // Getting normal hint size
    QApplication::connect(btn1, &QPushButton::clicked, [btn1](){
        QString title("Size hint");
        QString text = QString("SizeHint = (%1, %2)").arg(btn1->sizeHint().width()).arg(btn1->sizeHint().height());
        QMessageBox::information(nullptr, title, text, QMessageBox::Ok);
    });

    // Getting hint size more than btn1 because of long sign
    QApplication::connect(btn2, &QPushButton::clicked, [btn2](){
        QString title("Size hint");
        QString text = QString("SizeHint = (%1, %2)").arg(btn2->sizeHint().width()).arg(btn2->sizeHint().height());
        QMessageBox::information(nullptr, title, text, QMessageBox::Ok);

    });

    // Set up layout
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(btn1);
    layout->addWidget(btn2);
    wgt->setLayout(layout);

    return wgt;
}

QWidget* get_custom_progress(QWidget* parent){
    QWidget* wgt = new QWidget(parent);

    // Custom Widget
    CustomProgressBar* progress = new CustomProgressBar;
    QScrollBar* scroll = new QScrollBar(Qt::Horizontal);
    
    scroll->setRange(0, 100);
    progress->setRange(0, 100);

    // demonstrate custom progreess redraw
    QObject::connect(scroll, &QScrollBar::valueChanged, progress, &CustomProgressBar::setProgress);

    // Set up layout
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(progress);
    layout->addWidget(scroll);
    wgt->setLayout(layout);

    return wgt;
}



int main(int argc, char** argv)
{
    QApplication app(argc, argv); 

    QTabWidget wnd;
    wnd.addTab(get_hex_edit(&wnd), "Hex line edit");
    wnd.addTab(get_size_hints(&wnd), "Show size hint");
    wnd.addTab(get_custom_progress(&wnd), "Custom progress");

    wnd.resize(300, 200);

    wnd.show();

    return app.exec();
}
