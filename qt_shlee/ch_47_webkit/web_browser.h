#pragma once
#include <QWidget>

// This class is an (almost) complete web browser widget
class WebBrowser : public QWidget
{
    Q_OBJECT
public:
    // all stuff is encapsulated in constructor
    WebBrowser(QWidget* parent = nullptr);
};

