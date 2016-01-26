#pragma once
#include <QDeclarativeView>

class QtQuick1ApplicationViewer : public QDeclarativeView
{
    Q_OBJECT

public:
    enum ScreenOrientation {
        ScreenOrientationLockPortrait,
        ScreenOrientationLockLandscape,
        ScreenOrientationAuto
    };

    explicit QtQuick1ApplicationViewer(QWidget *parent = 0);
    virtual ~QtQuick1ApplicationViewer();

    static QtQuick1ApplicationViewer *create();

    void setMainQmlFile(const QString &file);
    void addImportPath(const QString &path);

    // Note that this will only have an effect on Fremantle.
    void setOrientation(ScreenOrientation orientation);

    void showExpanded();
};

