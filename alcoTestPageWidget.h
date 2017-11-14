#pragma once

#include <QWidget>

#include "page.h"

namespace Ui {
    class AlcoTestPageWidget;
}

class AlcoTestPageWidget : public Page
{
    Q_OBJECT

public:
    explicit AlcoTestPageWidget(QWidget *parent = 0);
    ~AlcoTestPageWidget();

    QString getName() const override;

protected:
    void initInterface() override;

private:
    Ui::AlcoTestPageWidget* _ui;
};
