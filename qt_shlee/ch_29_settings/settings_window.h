#pragma once
#include <QWidget>
#include <QSettings>

class QComboBox; 
class QCheckBox; 
class QTextEdit; 
class QLabel;

class SettingsWindow : public QWidget
{
    Q_OBJECT
public:
    SettingsWindow(QWidget* parent = nullptr);
    virtual ~SettingsWindow();

    void writeSettings() const;
    void readSettings();

    void checkBoxClicked();
    void comboBoxActivated(int);
private:

    // Settings accept company name (for Windows registry) and application name
    QSettings settings_;

    // appearance
    QComboBox* styles_combo_ = nullptr;
    QCheckBox* disable_edit_flag_ = nullptr;
    QTextEdit* editor_ = nullptr;
    QLabel* times_stared_ = nullptr;
    
    // load here how many times have we started
    int startts_counter_ = 0;
};

