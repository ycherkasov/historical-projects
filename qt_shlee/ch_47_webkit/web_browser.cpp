#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QLayout>
#include <QWebView>
#include <QWebHistory>
#include "web_browser.h"


WebBrowser::WebBrowser(QWidget* parent /*= nullptr*/) :
QWidget(parent)
{
    QWebView* browser = new QWebView;
    QLineEdit* address_bar = new QLineEdit("https://www.google.com/?gws_rd=ssl");
    QPushButton* back_button = new QPushButton("<-");
    QPushButton* forward_button = new QPushButton("->");

    // we can't go back-fwd just after start
    back_button->setEnabled(false);
    forward_button->setEnabled(false);

    // page load completion
    QProgressBar* load_progress = new QProgressBar;

    // Navigation buttons
    QPushButton* go_button = new QPushButton("&Go");
    QPushButton* stop_button = new QPushButton("&Stop");
    QPushButton* refresh_button = new QPushButton("&Refresh");

    // Auto-complete protocol
    auto lambda_go = [address_bar, browser](){
        if ((!address_bar->text().startsWith("ftp://")) ||
            (!address_bar->text().startsWith("http://")) ||
            (!address_bar->text().startsWith("https://"))){
            address_bar->setText(QString("%1%2").arg("http://").arg(address_bar->text()));
        }
        browser->load(QUrl(address_bar->text()));
    };

    auto lambda_load_finished = [browser, address_bar, back_button, forward_button](bool is_ok){
        
        // Error loading page
        if (!is_ok){
            browser->setHtml("<CENTER>An error has occurred while loading the web page</CENTER>");
        }

        // display loaded page URL
        address_bar->setText(browser->url().toString());

        // Enable bck-fwd if neccessary
        back_button->setEnabled(browser->page()->history()->canGoBack());
        forward_button->setEnabled(browser->page()->history()->canGoForward());
    };

    // go to page
    connect(go_button, &QPushButton::clicked, lambda_go);
    connect(address_bar, &QLineEdit::returnPressed, lambda_go);

    // navigation
    connect(back_button, &QPushButton::clicked, browser, &QWebView::back);
    connect(forward_button, &QPushButton::clicked, browser, &QWebView::forward);
    connect(refresh_button, &QPushButton::clicked, browser, &QWebView::reload);
    connect(stop_button, &QPushButton::clicked, browser, &QWebView::stop);

    // browser progress
    connect(browser, &QWebView::loadProgress, load_progress, &QProgressBar::setValue);
    connect(browser, &QWebView::loadFinished, lambda_load_finished);

    // Layout
    QHBoxLayout* panel_layout = new QHBoxLayout;
    panel_layout->addWidget(back_button);
    panel_layout->addWidget(forward_button);
    panel_layout->addWidget(stop_button);
    panel_layout->addWidget(refresh_button);
    panel_layout->addWidget(address_bar);
    panel_layout->addWidget(go_button);

    QVBoxLayout* browser_layout = new QVBoxLayout;
    browser_layout->addLayout(panel_layout);
    browser_layout->addWidget(browser);
    browser_layout->addWidget(load_progress);

    setLayout(browser_layout);

    // load default page
    lambda_go();
}
