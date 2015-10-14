#include <QRadioButton>
#include <QLayout>
#include <QWidget>

#include "slider_styles_group.h"

SliderStylesGroup::SliderStylesGroup(const QString& name, QWidget* parent) :
QGroupBox(name, parent)
, no_ticks_(new QRadioButton("&No ticks"))
, ticks_above_(new QRadioButton("Ticks &above"))
, ticks_below_(new QRadioButton("Ticks be&low"))
, ticks_both_(new QRadioButton("Ticks &both"))
{
    ticks_below_->setChecked(true);

    QVBoxLayout* radio_button_layout = new QVBoxLayout;
    radio_button_layout->addWidget(no_ticks_);
    radio_button_layout->addWidget(ticks_above_);
    radio_button_layout->addWidget(ticks_below_);
    radio_button_layout->addWidget(ticks_both_);
    this->setLayout(radio_button_layout);

    this->connect(no_ticks_, SIGNAL(clicked()), SLOT(style_changed_slot()));
    this->connect(ticks_above_, SIGNAL(clicked()), SLOT(style_changed_slot()));
    this->connect(ticks_below_, SIGNAL(clicked()), SLOT(style_changed_slot()));
    this->connect(ticks_both_, SIGNAL(clicked()), SLOT(style_changed_slot()));
}

void SliderStylesGroup::style_changed_slot()
{
    if (nullptr == external_slider_){
        return;
    }

    QSlider::TickPosition pos = QSlider::TicksBelow;

    if (no_ticks_->isChecked()){
        pos = QSlider::NoTicks;
    }
    else if (ticks_above_->isChecked()){
        pos = QSlider::TicksAbove;
    }
    else if (ticks_below_->isChecked()){
        pos = QSlider::TicksBelow;
    }
    else if (ticks_both_->isChecked()){
        pos = QSlider::TicksBothSides;
    }
    external_slider_->setTickPosition(pos);
}

void SliderStylesGroup::set_buddy_slider(QSlider* slider)
{
    external_slider_ = slider;
}
