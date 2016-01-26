#include <QAction>
#include <QApplication>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QPixmap>

#include "systray_window.h"

SystrayWindow::SystrayWindow(QWidget* parent /*= nullptr*/):
QLabel("<H1>Application Windows</H1>", parent)
{
    setWindowTitle("System Tray");


    // Menu actions
    QAction* show_hide_action = new QAction("&Show/Hide application window", this);
    QAction* show_message_action = new QAction("Show example &message", this);
    QAction* change_icon_action = new QAction("&Change tray icon", this);
    QAction* quit_action = new QAction("&Quit application", this);

    // ... bind them to slots
    connect(show_hide_action, &QAction::triggered, this, &SystrayWindow::showHideWindow);
    connect(show_message_action, &QAction::triggered, this, &SystrayWindow::showTrayMessage);
    connect(change_icon_action, &QAction::triggered, this, &SystrayWindow::changeTrayIcon);
    connect(quit_action, &QAction::triggered, qApp, &QApplication::quit);

    // ... create menu itself
    tray_menu_ = new QMenu(this);
    tray_menu_->addAction(show_hide_action);
    tray_menu_->addAction(show_message_action);
    tray_menu_->addAction(change_icon_action);
    tray_menu_->addAction(quit_action);

    // ... create icon
    tray_icon_ = new QSystemTrayIcon(this);
    tray_icon_->setContextMenu(tray_menu_);
    tray_icon_->setToolTip("System tray tooltip");

    changeTrayIcon();
    tray_icon_->show();
}

void SystrayWindow::showHideWindow()
{
    setVisible(!isVisible());
}

void SystrayWindow::showTrayMessage()
{
    tray_icon_->showMessage("Information message", "Just an example", QSystemTrayIcon::Information, 3000);
}

void SystrayWindow::changeTrayIcon()
{
    if (0 == tray_icon_index_){
        tray_icon_->setIcon(QPixmap("icon1.png"));
    }
    else if (1 == tray_icon_index_){
        tray_icon_->setIcon(QPixmap("icon2.png"));
    }
    else if (2 == tray_icon_index_){
        tray_icon_->setIcon(QPixmap("icon3.png"));
    }

    if (tray_icon_index_++ == 3){
        tray_icon_index_ = 0;
    }
}

void SystrayWindow::closeEvent(QCloseEvent * ev)
{
    // do not close app, just hide it on [x]
    if (tray_icon_->isVisible()){
        hide();
    }
}
