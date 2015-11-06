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

    // RAII wrapper for settings groups
    struct GroupGuard{
        
        GroupGuard(QSettings& settings, const QString& group_name) : 
            settings_(settings)
        {
            settings_.beginGroup(group_name);
        }
        
        ~GroupGuard(){
            settings_.endGroup();
        }

        GroupGuard(const GroupGuard&) = delete;
        GroupGuard& operator=(const GroupGuard&) = delete;

    private:
        QSettings& settings_;
    };

    SettingsWindow(QWidget* parent = nullptr);
    virtual ~SettingsWindow();

    // all methods changes QSetting, so they non-const
    void writeSettings();
    void readSettings();

    // enable-disable edit box
    void checkBoxClicked();

    // change edit style
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
    int starts_counter_ = 0;
};

