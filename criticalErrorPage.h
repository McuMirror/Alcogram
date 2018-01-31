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

    void init(MainWindow* mainWindow) override;
    QString getName() const override;

private:
    void onCriticalError(StateName fromState);
    void setCheckTimer();
    void onDevicesStatusOk();
    void onSomeDevicesStatusNotOk();
    void connectToDeviceChecker();
    void disconnectFromDeviceChecker();

    QTimer _checkTimer;
    StateName _fromState;
    Ui::CriticalErrorPage *ui;
};
