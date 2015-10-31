#include <QComboBox>
#include <QCheckBox>
#include <QTextEdit>
#include <QLabel>
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
}


SettingsWindow::~SettingsWindow()
{
}

void SettingsWindow::writeSettings() const
{

}

void SettingsWindow::readSettings()
{

}

void SettingsWindow::checkBoxClicked()
{

}

void SettingsWindow::comboBoxActivated(int)
{

}
