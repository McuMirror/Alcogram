#pragma once

#include <QMainWindow>

#include "configManager.h"

namespace Ui {
    class MainWindow;
}


enum PageName {
    NONCRITICAL_ERROR_PAGE = 0
    , CRITICAL_ERROR_PAGE = 1
    , SPLASH_SCREEN_NONCRITICAL_ERROR = 2
    , SPLASH_SCREEN_ETERNAL_SLEEP = 3
    , SPLASH_SCREEN = 4
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

private:
    // load fonts from resources
    void loadFonts();

    void removeFonts();

    Ui::MainWindow*_ui;
    QVector<int> _fonts;
    ConfigManager* _configManager;
};
