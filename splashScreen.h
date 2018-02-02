#pragma once

#include <QWidget>

#include "page.h"

namespace Ui {
    class SplashScreen;
}

class SplashScreen : public Page
{
    Q_OBJECT

public:
    explicit SplashScreen(QWidget *parent = 0);
    ~SplashScreen();

    void init(MainWindowInterface* mainWindow) override;
    QString getName() const override;
    QList<Transition*> getTransitions() override;

protected:
    void onEntry();

private:
    // callbacks for DeviceChecker signals
    void onDevicesStarted();
    void onSomeDevicesNotStarted();
    void onDevicesStatusOk();
    void onSomeDevicesStatusNotOk();
    void connectToDeviceChecker();
    void disconnectFromDeviceChecker();

    Ui::SplashScreen *_ui;
};
