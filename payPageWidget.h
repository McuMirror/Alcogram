#pragma once

#include <QWidget>
#include <QLabel>

#include "page.h"

namespace Ui {
class PayPageWidget;
}

class PayPageWidget : public Page
{
    Q_OBJECT

public:
    explicit PayPageWidget(QWidget *parent = 0);
    ~PayPageWidget();

    void init(MainWindow* mainWindow) override;

private:
    void initInterface();
    void initPriceLabels(QList<QLabel*> labels, const QString& richText);

    Ui::PayPageWidget* _ui;
};
