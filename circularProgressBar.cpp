#include <QPainter>
#include <QPen>

#include "circularProgressBar.h"

CircularProgressBar::CircularProgressBar(QWidget *parent) : QWidget(parent)
{

}

void CircularProgressBar::setProgress(int val)
{
    progress = (double)val/100;
    //yes, it is not very good, the best approach is to
    //create something similar to QProgressBar
    this->update();
}


void CircularProgressBar::paintEvent(QPaintEvent* paintEvent)
{
    QPainter p(this);

    //p.fillRect(0, 0, width(), height(), QBrush(QColor(255, 255, 255)));

    QLinearGradient gradient;
    gradient.setColorAt(0, QColor("#ffdd33"));
    gradient.setColorAt(1, QColor("#ffaa32"));

    QPen pen;
    pen.setWidth(10);
    pen.setColor(Qt::red);
    pen.setBrush(QBrush(gradient));
    p.setPen(pen);

    p.setRenderHint(QPainter::Antialiasing);

    QRectF rectangle(5, 5, width() - 10, height() - 10);
    //to understand these magic numbers, look drawArc method in Qt doc
    int startAngle = -90 * 16;
    int spanAngle = progress * 360 * 16;

    p.drawArc(rectangle, startAngle, spanAngle);
    //p.drawText(rectangle,Qt::AlignCenter,QString::number(progress*100)+" %");

}
