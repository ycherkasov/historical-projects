#include "toggle_button_group.h"
#include <QPushButton>
#include <QLayout>


ToggleButtonGroup::ToggleButtonGroup(const QString& name, QWidget* parent) :
QGroupBox(name, parent)
, toggle1_(new QPushButton("Toggle &1"))
, toggle2_(new QPushButton("Toggle &2"))
, toggle3_(new QPushButton("Toggle &3"))
{
    toggle1_->setCheckable(true);
    toggle2_->setCheckable(true);
    toggle3_->setCheckable(true);
    toggle1_->setChecked(true);

    QVBoxLayout* radio_button_layout = new QVBoxLayout;
    radio_button_layout->addWidget(toggle1_);
    radio_button_layout->addWidget(toggle2_);
    radio_button_layout->addWidget(toggle3_);
    this->setLayout(radio_button_layout);
}

void ToggleButtonGroup::toggle_changed_slot()
{

}
