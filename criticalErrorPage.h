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
    Ui::CriticalErrorPage *ui;
};
