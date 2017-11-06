#pragma once

#include <QWidget>

class CircularProgressBar : public QWidget
{
    Q_OBJECT
public:
    explicit CircularProgressBar(QWidget *parent = 0);

public slots:
    void setProgress(int val);

protected:
    void paintEvent(QPaintEvent* paintEvent);

private:
    double progress = 1;
};
