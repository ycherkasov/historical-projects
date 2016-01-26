#pragma once
#include <QGroupBox>
#include <QSlider>

class QRadioButton;
class QString;

class SliderStylesGroup : public QGroupBox
{
    Q_OBJECT
public:
    SliderStylesGroup(const QString& name, QWidget* parent = nullptr);
    
    SliderStylesGroup(const SliderStylesGroup&) = delete;
    SliderStylesGroup& operator=(const SliderStylesGroup&) = delete;

    void set_buddy_slider(QSlider* slider);

public slots:
    void style_changed_slot();

private:
    QRadioButton* no_ticks_;
    QRadioButton* ticks_above_;
    QRadioButton* ticks_below_;
    QRadioButton* ticks_both_;
    QSlider* external_slider_ = nullptr;
};

