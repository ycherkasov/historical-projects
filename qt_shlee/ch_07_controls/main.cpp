#include <QApplication>
#include <QtGui>
#include <QLayout>
#include <QProgressBar>
#include <QDial>
#include <QLCDNumber>
#include <QScrollBar>
#include <QLabel>

#include "slider_styles_group.h"
#include "toggle_button_group.h"

// TODO: stretch only 3rd row. Why?

// * QLabel is QFrame child and may have a frame
// * QLabels are HTML-aware, including pictures and HREFs
//   SetOpenExternalLink() must be called
// * QLabels may hold pictures and movies
// * Widget text could contain & sign for Alt-Hotkey
// * QLabel::SetBuddy() moves tab order and hotkeys to the buddy widget
// * QLCD supports bin/oct/dec/hex, different digit styles
// * QAbscractButton is a base for buttons, check-boxes and radio
// * Button signals - clicked, pressed, released, toggled (for toggle-styled)
// * QPushButton::SetMenu() accociate popup menu
// * QCheckBox has up to 3 states
// * QSlider::setTracking() - immediate processing or after mouse button release
// * QScrollBar used separately very rare. Use QAbstractScrollArea instead
// * QDial difference from QProgressBar is ability to "jump" min<->max



int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QWidget wnd;

    // 1 row
    SliderStylesGroup* slider_group = new SliderStylesGroup("Slider styles", &wnd);
    QSlider* slider = new QSlider(Qt::Horizontal);
    slider->setRange(0, 100);
    slider->setValue(0);
    slider->setTickInterval(10);
    slider->setTickPosition(QSlider::TicksBelow);

    slider_group->set_buddy_slider(slider);

    // 2 row
    ToggleButtonGroup* toggle = new ToggleButtonGroup("Toggle buttons", &wnd);
    QProgressBar* progress = new QProgressBar;
    progress->setRange(0, 100);
    progress->setValue(0);


    // 3 row
    QDial* dial = new QDial(&wnd);
    dial->setRange(0, 100);
    dial->setValue(0);
    dial->setNotchTarget(10);
    dial->setNotchesVisible(true);

    QLCDNumber* lcd = new QLCDNumber(8);
    lcd->setSegmentStyle(QLCDNumber::Flat);
    lcd->display(0);
    QScrollBar* scroll = new QScrollBar(Qt::Horizontal);
    scroll->setRange(0, 100);
    scroll->setValue(0);

    QVBoxLayout* int_layout = new QVBoxLayout;
    int_layout->addWidget(lcd);
    int_layout->addWidget(scroll);

    // 4 row - HTML Aware label
    QLabel* lbl = new QLabel;
    // Formatted string
    QString s = QString("<H1><CENTER>QLabel - HTML Demo</CENTER></H1>"

        "<H2><CENTER>List</CENTER><H2>"
        "<OL><LI>One</LI>"
        " <LI>Two</LI>"
        " <LI>Three</LI>"
        "</OL>"

        "<H2><CENTER>Font Style</CENTER><H2>"
        "<CENTER><FONT COLOR=RED>"
        " <B>Bold</B>, <I>Italic</I>, <U>Underline</U>"
        "</FONT></CENTER>"

        "<H2><CENTER>Table</CENTER></H2>"
        "<CENTER> <TABLE>"
        "<TR BGCOLOR = #ff00ff>"
        "<TD>%1</TD><TD>%1</TD><TD>%1</TD><TD>%1</TD>"
        "< / TR>"
        "<TR BGCOLOR = YELLOW>"
        "<TD>%2</TD> <TD>%2</TD> <TD>%2</TD> <TD>%2</TD > "
        "</TR>"
        "<TR BGCOLOR = #00f000>"
        "<TD>3,1</TD><TD>3,2</TD><TD>3,3</TD><TD>3,4</TD>"
        "</TABLE> </CENTER>").arg(1).arg(2);
    lbl->setText(s);

    // Layout
    QGridLayout* grid = new QGridLayout;
    grid->setMargin(5);
    grid->setSpacing(15);

    grid->addWidget(slider_group, 0, 0, 1, 1);
    grid->addWidget(slider, 0, 1, 1, 3);
    
    grid->addWidget(toggle, 1, 0, 1, 1);
    grid->addWidget(progress, 1, 1, 1, 3);
    
    grid->addWidget(dial, 2, 0, 1, 1);
    grid->addLayout(int_layout, 2, 1);

    grid->addWidget(lbl, 3, 0, 1, 3);
    

    wnd.setLayout(grid);

    QObject::connect(slider, SIGNAL(valueChanged(int)), dial, SLOT(setValue(int)));
    QObject::connect(slider, SIGNAL(valueChanged(int)), lcd, SLOT(display(int)));
    QObject::connect(dial, SIGNAL(valueChanged(int)), progress, SLOT(setValue(int)));
    QObject::connect(dial, SIGNAL(valueChanged(int)), scroll, SLOT(setValue(int)));

    wnd.setMinimumWidth(500);
    wnd.show();

    return app.exec();
}