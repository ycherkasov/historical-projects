#pragma once
#include <QLabel>

class QSystemTrayIcon; 
class QMenu;

// Application Window that supports systray icon and menu
class SystrayWindow : public QLabel
{
    Q_OBJECT
public:
    SystrayWindow(QWidget* parent = nullptr);

    void showHideWindow();
    void showTrayMessage();
    void changeTrayIcon();

protected:

    // catch close event so that
    // do not close app, just hide it on [x]
    virtual void closeEvent(QCloseEvent * ev);

private:
    QSystemTrayIcon* tray_icon_;
    QMenu* tray_menu_;
    int tray_icon_index_ = 0;
};

