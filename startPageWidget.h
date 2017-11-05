#pragma once

#include <QWidget>

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

    void init(MainWindow* mainWindow) override;

private slots:
    void onStartButtonRelease();

private:
    void initInterface(const QString& family);

    Ui::StartPageWidget* _ui;
    MainWindow* _mainWindow;
};
