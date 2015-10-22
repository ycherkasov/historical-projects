#include "toggle_list_styles.h"
#include <QRadioButton>
#include <QListWidget>
#include <QLayout>


ToggleListStyles::ToggleListStyles(const QString& name, QWidget* parent /*= nullptr*/) :
    QGroupBox(name, parent),
    plain_list_(new QRadioButton("&Plain list")),
    editable_list_(new QRadioButton("&Editable list")),
    drag_enabled_list_(new QRadioButton("&Drag enabled list"))
{
    plain_list_->setChecked(true);

    QVBoxLayout* radio_button_layout = new QVBoxLayout;
    radio_button_layout->addWidget(plain_list_);
    radio_button_layout->addWidget(editable_list_);
    radio_button_layout->addWidget(drag_enabled_list_);
    this->setLayout(radio_button_layout);

    this->connect(plain_list_, &QRadioButton::clicked, this, &ToggleListStyles::style_changed_slot);
    this->connect(editable_list_, &QRadioButton::clicked, this, &ToggleListStyles::style_changed_slot);
    this->connect(drag_enabled_list_, &QRadioButton::clicked, this, &ToggleListStyles::style_changed_slot);
}

void ToggleListStyles::set_buddy_list(QListWidget* l)
{
    buddy_list_ = l;
}

void ToggleListStyles::style_changed_slot()
{
    if (nullptr == buddy_list_){
        return;
    }

    QStringList lst;
    lst << "First" << "Second" << "Third";

    buddy_list_->clear();

    for (const QString& s : lst){
        QListWidgetItem* item = new QListWidgetItem(s, buddy_list_);
        

        if (plain_list_->isChecked()){
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }

        if (editable_list_->isChecked()){
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
        }

        if (drag_enabled_list_->isChecked()){

            // TODO: unclear for a while
            // try icons/string with icons
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
        }

        // TODO: try single selection/no selection

        // insertItem/insertItems are possible
    }

}
