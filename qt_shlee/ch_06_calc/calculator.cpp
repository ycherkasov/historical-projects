#include "calculator.h"
#include <QLCDNumber>
#include <QPushButton>
#include <QLayout>
#include <QRegExp>


QChar Calculator::buttons[4][4] = 
{
    { '7', '8', '9', '/' },
    { '4', '5', '6', '*' },
    { '1', '2', '3', '-' },
    { '0', '.', '=', '+' }
};

Calculator::Calculator(QWidget* parent/* = nullptr*/) :
    display_(new QLCDNumber(12)){

    // display setup
    display_->setSegmentStyle(QLCDNumber::Flat);
    display_->setMinimumSize(150, 50);

    // layout setup
    QGridLayout* calc_layout = new QGridLayout;
    calc_layout->addWidget(display_, 0, 0, 1, 4);
    calc_layout->addWidget(create_button("CE"), 1, 3);

    // keypad setup
    for (size_t i = 0; i < 4; ++i){
        for (size_t j = 0; j < 4; ++j){
            calc_layout->addWidget(create_button(buttons[i][j]), i + 2, j);
        }
    }

    // finish setup
    setLayout(calc_layout);
}


Calculator::~Calculator(){}

QPushButton*  Calculator::create_button(const QString& btn_name)
{
    QPushButton* new_button = new QPushButton(btn_name);
    new_button->setMinimumSize(40, 40);
    // method analog of signal-slot connection
    // 3-rd param is 'this'
    connect(new_button, SIGNAL(clicked()), SLOT(on_button_clicked()));
    return new_button;
}

void Calculator::calculate()
{
    double op1 = input_stack_.pop().toDouble();
    QString op = input_stack_.pop();
    double op2 = input_stack_.pop().toDouble();
    double result = 0;

    if (op == "+"){
        result = op1 + op2;
    }
    if (op == "-"){
        result = op1 - op2;
    }
    if (op == "/"){
        result = op1 / op2;
    }
    if (op == "*"){
        result = op1 * op2;
    }

    display_->display(result);
}

void Calculator::on_button_clicked()
{
    // we ensure that the sender is a PushButton
    QPushButton* pressed_btn = dynamic_cast<QPushButton*>(sender());
    
    if (!pressed_btn){
        return;
    }

    QString cmd = pressed_btn->text();

    if (cmd == "CE"){
        input_stack_.clear();
        str_display_ = "";
        display_->display("0");
        return;
    }

    if (cmd.contains(QRegExp("[0-9]"))){
        str_display_ += cmd;
        display_->display(str_display_.toDouble());
    }
    else if (cmd == "."){
        str_display_ += cmd;
        display_->display(str_display_);
    }
    else {
        if (input_stack_.count() >= 2){
            input_stack_.push(QString().setNum(display_->value()));
            calculate();
            input_stack_.clear();
            input_stack_.push(QString().setNum(display_->value()));
            if (cmd != "="){
                input_stack_.push(cmd);
            }
        }
        else{
            input_stack_.push(QString().setNum(display_->value()));
            str_display_ = "";
            display_->display("0");
        }
    }
}
