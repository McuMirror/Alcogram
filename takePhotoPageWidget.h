#pragma once

#include <QWidget>
#include <QTimer>

#include "page.h"

namespace Ui {
    class TakePhotoPageWidget;
}

class TakePhotoPageWidget : public Page
{
    Q_OBJECT

public:
    explicit TakePhotoPageWidget(QWidget *parent = 0);
    ~TakePhotoPageWidget();

    QString getName() const override;
    QList<Transition*> getTransitions() override;
    void onEntry() override;

protected:
    void initInterface() override;
    void setConnections() override;

private:
    // setting photo timer with duration named "timer"
    void setPhotoTimer();

    // setting inaction timer with duration named durationName
    void setInactionTimer(const QString& durationName);

    // returns bottomPanel page numbers related to specific state
    QMap<StateName, int> initBottomPanelPageNumbers() const;

    // returns mainPanel page numbers related to specific state
    QMap<StateName, int> initMainPanelPageNumbers() const;

    // setting page for bottomPanel and mainPanel by stateName
    void setSubPage(StateName stateName);

    Ui::TakePhotoPageWidget* _ui;
    QTimer _timer;
    int _timerTimeLeft; // remaining time for photo timer

    QMap<StateName, int> _bottomPanelPageNumbers; // state -> bottomPanel page number
    QMap<StateName, int> _mainPanelPageNumbers; // state -> mainPanel page number
};
