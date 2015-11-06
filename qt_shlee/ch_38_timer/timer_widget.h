#pragma once
#include <QLabel>

class QComboBox;
class QLabel;

// This class creates QTimer object and process event every 500 msec
class TimerWidget : public QLabel
{
public:
    TimerWidget(QWidget* parent = nullptr);
};

