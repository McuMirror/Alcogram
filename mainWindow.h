#pragma once

#include <QMainWindow>

#include "configManager.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void nextPage();
    ConfigManager* getConfigManager() const;

private:
    void loadFonts();
    void removeFonts();

    Ui::MainWindow*_ui;
    QVector<int> _fonts;
    ConfigManager* _configManager;
};
