#include "date_time_widget.h"
#include <QComboBox>
#include <QLabel>
#include <QDate>
#include <QTime>
#include <QLayout>


DateTimeWidget::DateTimeWidget(QWidget* parent /*= nullptr*/):
QWidget(parent)
{
    QComboBox* date_formats = new QComboBox;
    date_formats->addItem("d.M.yy");
    date_formats->addItem("dd/MM/yy");
    date_formats->addItem("yyyy.MMM.ddd");
    date_formats->addItem("yyyy.MMMM.dddd");
    date_formats->setCurrentIndex(0);
    // see also QDate reference

    // date / time format could be text or predefined(DateFormat enum)
    QLabel* date_label = new QLabel(QDate::currentDate().toString("d.M.yy"));

    QComboBox* time_formats = new QComboBox;
    time_formats->addItem("h:m:s ap");
    time_formats->addItem("hh:mm:ss.zzz");
    time_formats->setCurrentIndex(0);

    QLabel* time_label = new QLabel(QTime::currentTime().toString("h:m:s ap"));

    // set Layout
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(date_formats);
    layout->addWidget(date_label);
    layout->addWidget(time_formats);
    layout->addWidget(time_label);
    setLayout(layout);


    // new connect() syntax with overload!
    QObject::connect(date_formats, 
        static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        date_label, 
        [date_formats, date_label](int){
        date_label->setText(QDate::currentDate().toString(date_formats->currentText()));
    });

    QObject::connect(time_formats,
        static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        time_label,
        [time_formats, time_label](int){
        time_label->setText(QTime::currentTime().toString(time_formats->currentText()));
    });

}
