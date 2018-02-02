#pragma once

#include <QWidget>

#include "page.h"

namespace Ui {
    class NoncriticalErrorPage;
}

class NoncriticalErrorPage : public Page
{
    Q_OBJECT

public:
    explicit NoncriticalErrorPage(QWidget *parent = 0);
    ~NoncriticalErrorPage();

    void init(MainWindowInterface* mainWindow) override;
    QString getName() const override;

private:
    Ui::NoncriticalErrorPage *ui;
};
