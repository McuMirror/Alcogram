#pragma once

#include <QWidget>

#include "page.h"

namespace Ui {
class PhotoPrintPageWidget;
}

class PhotoPrintPageWidget : public Page
{
    Q_OBJECT

public:
    explicit PhotoPrintPageWidget(QWidget *parent = 0);
    ~PhotoPrintPageWidget();

    QString getName() const override;

protected:
    void initInterface() override;

private:

    Ui::PhotoPrintPageWidget* _ui;
};
