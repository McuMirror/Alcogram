#pragma once

#include <QMainWindow>

#include "configManager.h"
#include "stateMachine.h"

namespace Ui {
    class MainWindow;
}


enum PageName {
    NONCRITICAL_ERROR_PAGE = 0
    , CRITICAL_ERROR_PAGE = 1
    , SPLASH_SCREEN_NONCRITICAL_ERROR_PAGE = 2
    , SPLASH_SCREEN_ETERNAL_SLEEP_PAGE = 3
    , SPLASH_SCREEN_PAGE = 4
    , START_PAGE = 5
    , TAKE_PHOTO_PAGE = 6
    , PAY_PAGE = 7
    , ALCOTEST_PAGE = 8
    , PHOTO_PRINT_PAGE = 9
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setPage(PageName pageName);
    ConfigManager* getConfigManager() const;
    void postEvent(Event* event);

private:
    // load fonts from resources
    void loadFonts();

    void removeFonts();

    Ui::MainWindow*_ui;
    QVector<int> _fonts;
    ConfigManager* _configManager;
    StateMachine* _stateMachine;
};
