#include <QComboBox>
#include <QCheckBox>
#include <QTextEdit>
#include <QLabel>
#include <QLayout>
#include <QApplication>

#include "settings_window.h"


SettingsWindow::SettingsWindow(QWidget* parent/* = nullptr*/) :
QWidget(parent),
settings_("Atatat LTD", "Program with settings")
{
    styles_combo_ = new QComboBox;
    disable_edit_flag_ = new QCheckBox("Disable edit");
    editor_ = new QTextEdit;
    times_stared_ = new QLabel;

    styles_combo_->addItem("Classic");
    styles_combo_->addItem("Borland");

    QApplication::connect(disable_edit_flag_, &QCheckBox::clicked, this, &SettingsWindow::checkBoxClicked);
    QApplication::connect(styles_combo_, SIGNAL(activated()), this, SLOT(comboBoxActivated()));

    readSettings();

    // Setup vertical layout
    QVBoxLayout* vertical_layout = new QVBoxLayout;
    vertical_layout->setMargin(5);
    vertical_layout->setSpacing(15);
    vertical_layout->addWidget(times_stared_);
    vertical_layout->addWidget(editor_);

    // Setup horizontal layout
    QHBoxLayout* horz_layout = new QHBoxLayout;
    horz_layout->setSpacing(15);
    horz_layout->addWidget(styles_combo_);
    horz_layout->addWidget(disable_edit_flag_);
    
    // embed
    vertical_layout->addLayout(horz_layout);
    setLayout(vertical_layout);
}


SettingsWindow::~SettingsWindow()
{
    writeSettings();
}

void SettingsWindow::writeSettings()
{
    GroupGuard guard(settings_, "/Settings");

    settings_.setValue("/text", editor_->toPlainText());
    settings_.setValue("/style", styles_combo_->currentIndex());
    settings_.setValue("/disabled", disable_edit_flag_->isChecked());
    settings_.setValue("/width", width());
    settings_.setValue("/height", height());
    settings_.setValue("/counter", starts_counter_);
}

void SettingsWindow::readSettings()
{
    GroupGuard guard(settings_, "/Settings");

    // Controls state
    QString edit_text = settings_.value("/text", "").toString();
    int style = settings_.value("/style", 0).toInt();
    bool is_disabled = settings_.value("/disabled", false).toBool();

    // Window size
    int saved_width = settings_.value("/width", width()).toInt();
    int saved_height = settings_.value("/height", height()).toInt();

    starts_counter_ = settings_.value("/counter", 1).toInt();

    QString label_val = QString("This application has been started %1 time(s)").arg(starts_counter_++);
    times_stared_->setText(label_val);

    editor_->setPlainText(edit_text);

    resize(saved_width, saved_height);

    disable_edit_flag_->setChecked(is_disabled);
    checkBoxClicked();

    styles_combo_->setCurrentIndex(style);
    comboBoxActivated(style);
}

void SettingsWindow::checkBoxClicked()
{
    editor_->setEnabled(!disable_edit_flag_->isChecked());
}

void SettingsWindow::comboBoxActivated(int index)
{
    QPalette p = editor_->palette();
    QColor base_color = Qt::white;
    QColor text_color = Qt::black;

    if (0 == index){
        base_color = Qt::white;
        text_color = Qt::black;
    }
    else if (1 == index){
        base_color = Qt::darkBlue;
        text_color = Qt::yellow;
    }

    p.setColor(QPalette::Active, QPalette::Base, base_color);
    p.setColor(QPalette::Active, QPalette::Text, text_color);

    editor_->setPalette(p);
}
