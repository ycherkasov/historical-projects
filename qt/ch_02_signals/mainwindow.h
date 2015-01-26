#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class ExternalDialog;

class SignalSlotCounter;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    // set property so that support
    // obj->setProperty("readOnly") semantic
    Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly)

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushOpenDialogButton_clicked();

private:
    Ui::MainWindow *ui;

    SignalSlotCounter* m_counter;
    ExternalDialog* m_extDialog;

    // member variable for the property set above
    bool m_readOnly;

    bool readOnly() const
    {
        return m_readOnly;
    }

    void setReadOnly(bool ro)
    {
        m_readOnly = ro;
    }
};

#endif // MAINWINDOW_H
