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

    void init(MainWindow* mainWindow) override;
    QString getName() const override;

private:
    void initInterface();

    Ui::PhotoPrintPageWidget* _ui;
};
