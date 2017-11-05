#pragma once

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString getFontFamily();
    void nextPage();

private:
    void loadFonts();
    void removeFonts();

    Ui::MainWindow*_ui;
    QVector<int> _fonts;
    QString _fontFamily;
};
