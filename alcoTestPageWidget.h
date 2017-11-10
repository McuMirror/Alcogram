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

    void init(MainWindow* mainWindow) override;
    QString getName() const override;

private:
    void initInterface();

    Ui::AlcoTestPageWidget* _ui;
};
