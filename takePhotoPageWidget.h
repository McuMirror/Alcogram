#pragma once

#include <QWidget>

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

    void init(MainWindow* mainWindow) override;

private:
    void initInterface(const QString& family);

    Ui::TakePhotoPageWidget* _ui;
};
