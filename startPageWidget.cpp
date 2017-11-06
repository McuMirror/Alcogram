#include <QGraphicsDropShadowEffect>
#include <QFontDatabase>

#include "startPageWidget.h"
#include "ui_startPageWidget.h"
#include "utils.h"

StartPageWidget::StartPageWidget(QWidget *parent) :
    Page(parent),
    _ui(new Ui::StartPageWidget)
{
    _ui->setupUi(this);
}

void StartPageWidget::init(MainWindow* mainWindow) {
    _mainWindow = mainWindow;

    initInterface();

    QObject::connect(_ui->startButton, &QPushButton::released, this, &StartPageWidget::onStartButtonRelease);
}

void StartPageWidget::onStartButtonRelease() {
    _mainWindow->nextPage();
}

void StartPageWidget::initInterface() {
    //startButton font
    _ui->startButton->setFont(Utils::getFont("Proxima Nova Rg", 28, 4, QFont::Bold));

    //startButton shadow
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);

    effect->setBlurRadius(142);
    effect->setColor(QColor(0, 0, 0, 117));
    effect->setOffset(0, 57);
    _ui->startButton->setGraphicsEffect(effect);

    //drunk womens photo shadow
    effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(128);
    effect->setColor(QColor(0, 0, 0, 127));
    effect->setOffset(0, 42);
    _ui->drunkWomensPhoto->setGraphicsEffect(effect);

    //text 01, 02, 03 font
    QFont font = Utils::getFont("Proxima Nova Rg", 200, 0, QFont::Bold);

    _ui->label01->setFont(font);
    _ui->label02->setFont(font);
    _ui->label03->setFont(font);

    //step descriptions font
    font = Utils::getFont("Proxima Nova Rg", 29, 0, QFont::Bold);

    _ui->firstStepDescription->setFont(font);
    _ui->sndStepDescription->setFont(font);
    _ui->thirdStepDescription->setFont(font);

    //switch language button font
    _ui->switchLanguageButton->setFont(font);

    //alcogram text
    _ui->alcogramText->setTextFormat(Qt::RichText);
    _ui->alcogramText->setText("<html>"
                               "<head/>"
                               "<body>"
                               "<p>"
                               "<span style=\"font-size:28px; font-weight:600; font-family:Proxima Nova Rg; \">AL</span>"
                               "<span style=\"font-size:28px; font-weight:600; font-family:Proxima Nova Lt; \">CO</span>"
                               "<span style=\"font-size:28px; font-family:Proxima Nova Rg; \">GR</span>"
                               "<span style=\"font-size:28px; font-family:Proxima Nova Lt; \">AM</span>"
                               "</p>"
                               "</body>"
                               "</html>");
}

StartPageWidget::~StartPageWidget()
{
    delete _ui;
}
