#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "signalslotcounter.h"
#include "externaldialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_counter = new SignalSlotCounter();
    m_extDialog = new ExternalDialog(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushOpenDialogButton_clicked()
{
    m_extDialog->setModal(true);
    m_extDialog->show();
}
