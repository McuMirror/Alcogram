#include "startPageWidget.h"
#include "ui_startPageWidget.h"

#include <QGraphicsDropShadowEffect>
#include <QFontDatabase>

StartPageWidget::StartPageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartPageWidget)
{
    ui->setupUi(this);


    int fontId = QFontDatabase::addApplicationFont(":/fonts/fonts/proximanova_bold.ttf");
    QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);

    QFont font(family);
    font.setPixelSize(28);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 4);
    font.setBold(true);

    ui->startButton->setFont(font);

    QPalette* startButtonPalette = new QPalette();

    startButtonPalette->setColor(QPalette::ButtonText, QColor("#050505"));

    ui->startButton->setPalette(*startButtonPalette);

    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);

    effect->setBlurRadius(142);
    effect->setColor(QColor(0, 0, 0, 117));
    effect->setOffset(0, 57);
    ui->startButton->setGraphicsEffect(effect);


    QFont label0Font(family);
    label0Font.setPixelSize(200);
    label0Font.setBold(true);

    ui->label01->setFont(label0Font);
    ui->label02->setFont(label0Font);
    ui->label03->setFont(label0Font);
}

void StartPageWidget::initInterface() {
    int fontId = QFontDatabase::addApplicationFont(":/fonts/fonts/proximanova_bold.ttf");
    QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);

    //setting startButton font and text color
    QFont font(family);
    font.setPixelSize(28);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 4);
    font.setBold(true);

    ui->startButton->setFont(font);

    QPalette* startButtonPalette = new QPalette();

    startButtonPalette->setColor(QPalette::ButtonText, QColor("#050505"));

    ui->startButton->setPalette(*startButtonPalette);


    //startButton shadow
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);

    effect->setBlurRadius(142);
    effect->setColor(QColor(0, 0, 0, 117));
    effect->setOffset(0, 57);
    ui->startButton->setGraphicsEffect(effect);

    //setting font for text 01, 02, 03
    font = QFont(family);
    font.setPixelSize(200);
    font.setBold(true);

    ui->label01->setFont(font);
    ui->label02->setFont(font);
    ui->label03->setFont(font);

}

StartPageWidget::~StartPageWidget()
{
    delete ui;
}
