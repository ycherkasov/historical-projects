#include "toggle_list_styles.h"
#include <QRadioButton>
#include <QListWidget>
#include <QLayout>


ToggleListStyles::ToggleListStyles(const QString& name, QWidget* parent /*= nullptr*/) :
    QGroupBox(name, parent),
    plain_list_(new QRadioButton("&Plain list")),
    editable_list_(new QRadioButton("&Editable list")),
    drag_enabled_list_(new QRadioButton("&Drag enabled list")),
    icons_list_(new QRadioButton("&Icons list")),
    icons_text_list_(new QRadioButton("&Text and icons list"))
{
    plain_list_->setChecked(true);

    // TODO: manage it in future
    drag_enabled_list_->setEnabled(false);

    QVBoxLayout* radio_button_layout = new QVBoxLayout;
    radio_button_layout->addWidget(plain_list_);
    radio_button_layout->addWidget(editable_list_);
    radio_button_layout->addWidget(drag_enabled_list_);
    radio_button_layout->addWidget(icons_list_);
    radio_button_layout->addWidget(icons_text_list_);
    this->setLayout(radio_button_layout);

    this->connect(plain_list_, &QRadioButton::clicked, this, &ToggleListStyles::style_changed_slot);
    this->connect(editable_list_, &QRadioButton::clicked, this, &ToggleListStyles::style_changed_slot);
    this->connect(drag_enabled_list_, &QRadioButton::clicked, this, &ToggleListStyles::style_changed_slot);
    this->connect(icons_list_, &QRadioButton::clicked, this, &ToggleListStyles::style_changed_slot);
    this->connect(icons_text_list_, &QRadioButton::clicked, this, &ToggleListStyles::style_changed_slot);
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
    QStringList icons;
    lst << "First" << "Second" << "Third";
    icons << "drive_win7.png" << "directory_win7.png" << "control_win7.png";

    buddy_list_->clear();

    for (size_t i = 0; i < 3; ++i){
        QListWidgetItem* item = new QListWidgetItem(buddy_list_);
        

        if (plain_list_->isChecked()){
            item->setText(lst[i]);
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }

        if (editable_list_->isChecked()){
            item->setText(lst[i]);
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
        }

        if (drag_enabled_list_->isChecked()){

            // TODO: unclear for a while
            item->setText(lst[i]);
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
        }

        if (icons_list_->isChecked()){
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable );
            item->setIcon(QPixmap(icons[i]));
        }

        if (icons_text_list_->isChecked()){
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            item->setIcon(QPixmap(icons[i]));
            item->setText(lst[i]);
        }

        // TODO: try single selection/no selection

        // insertItem/insertItems are possible
    }

}
