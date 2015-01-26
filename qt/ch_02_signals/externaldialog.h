#ifndef EXTERNALDIALOG_H
#define EXTERNALDIALOG_H

#include <QDialog>

namespace Ui {
class ExternalDialog;
}

class ExternalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExternalDialog(QWidget *parent = 0);
    ~ExternalDialog();

private:
    Ui::ExternalDialog *ui;
};

#endif // EXTERNALDIALOG_H
