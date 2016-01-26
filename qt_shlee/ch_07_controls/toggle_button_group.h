#pragma once
#include <QGroupBox>

class QPushButton;

class ToggleButtonGroup : public QGroupBox
{
    Q_OBJECT
public:
    ToggleButtonGroup(const QString& name, QWidget* parent = nullptr);

    ToggleButtonGroup(const ToggleButtonGroup&) = delete;
    ToggleButtonGroup& operator=(const ToggleButtonGroup&) = delete;


public slots:
    void toggle_changed_slot();

private:
    QPushButton* toggle1_;
    QPushButton* toggle2_;
    QPushButton* toggle3_;
};

