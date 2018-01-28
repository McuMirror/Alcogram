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

    Ui::StartPageWidget* _ui;
};
