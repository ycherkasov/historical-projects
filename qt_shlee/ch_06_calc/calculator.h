#pragma once
#include <QWidget>
#include <QStack>
#include <QString>

class QLCDNumber;
class QPushButton;

class Calculator : public QWidget
{
    Q_OBJECT
public:
    Calculator(QWidget* parent = nullptr);
    virtual ~Calculator();

public slots:
    void on_button_clicked();

protected:
    QPushButton* create_button(const QString& btn_name);
    void calculate();
private:
    QLCDNumber* display_;
    QStack<QString> results_;
    QString str_display_;
    static QChar buttons[4][4];
};

