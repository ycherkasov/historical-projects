#pragma once
#include <QGroupBox>

class QRadioButton;
class QListWidget;

class ToggleListStyles : public QGroupBox
{
    Q_OBJECT
public:
    ToggleListStyles(const QString& name, QWidget* parent = nullptr);

    ToggleListStyles(const ToggleListStyles&) = delete;
    ToggleListStyles& operator=(const ToggleListStyles&) = delete;

    void set_buddy_list(QListWidget* l);

public slots:
    void style_changed_slot();

private:
    QRadioButton* plain_list_;
    QRadioButton* editable_list_;
    QRadioButton* drag_enabled_list_;
    QListWidget* buddy_list_ = nullptr;

};

