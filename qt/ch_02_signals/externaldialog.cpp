#include "externaldialog.h"
#include "ui_externaldialog.h"

ExternalDialog::ExternalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExternalDialog)
{
    ui->setupUi(this);
    //connect(ui->pushIncrementButton, SIGNAL(clicked()),);

}

ExternalDialog::~ExternalDialog()
{
    delete ui;
}
