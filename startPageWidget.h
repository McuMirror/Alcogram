#pragma once

#include <QWidget>
#include <QTimer>

#include "page.h"

namespace Ui {
    class StartPageWidget;
}


class StartPageWidget : public Page
{
    Q_OBJECT

public:
    explicit StartPageWidget(QWidget *parent = 0);
    ~StartPageWidget();

    QString getName() const override;
    QList<Transition*> getTransitions() override;

protected:
    void onEntry() override;
    void initInterface() override;
    void setConnections() override;

private slots:
    void onSwitchLanguageButtonRelease();

private:
    // callbacks for DeviceChecker signals
    void onDevicesConnected();
    void onSomeDevicesNotConnected();
    void checkDevicesConnection();
    void disconnectFromDevicesChecker();

    static const int ERROR_SUBPAGE = 1;
    static const int MAIN_SUBPAGE = 0;

    int _checkConnectionAttempt; // number of attempts to check connection with devices in succession
    bool _connectedToDeviceChecker;
    Ui::StartPageWidget* _ui;
};
