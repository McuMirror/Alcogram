#pragma once

#include <QWidget>

#include "page.h"

namespace Ui {
    class CriticalErrorPage;
}

class CriticalErrorPage : public Page
{
    Q_OBJECT

public:
    explicit CriticalErrorPage(QWidget *parent = 0);
    ~CriticalErrorPage();

    void init(MainWindowInterface* mainWindow) override;
    QString getName() const override;

public:
    QList<Transition *> getTransitions() override;

private:
    // stot for signal MainWindow::criticalError
    void onCriticalError(StateName fromState);

    // set timer for device checking
    void setCheckTimer();

    // callback for DeviceChecker signals for check status operation
    void onDevicesStatusOk();
    void onSomeDevicesStatusNotOk();

    // connect/disconnect from DeviceChecker signals
    void connectToDeviceChecker();
    void disconnectFromDeviceChecker();

    QTimer _checkTimer;
    StateName _fromState; // the state where the critical error occurred
    Ui::CriticalErrorPage *ui;
};
