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
    QString getName() const override;
    QList<TransitionPack> getTransitions() override;

private:
    void initInterface();

    Ui::TakePhotoPageWidget* _ui;
};
