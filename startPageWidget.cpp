#include <QGraphicsDropShadowEffect>
#include <QFontDatabase>

#include "startPageWidget.h"
#include "ui_startPageWidget.h"
#include "utils.h"
#include "configManager.h"

StartPageWidget::StartPageWidget(QWidget *parent) :
    Page(parent),
    _ui(new Ui::StartPageWidget)
{
    _ui->setupUi(this);
}

void StartPageWidget::init(MainWindow* mainWindow)
{
    Page::init(mainWindow);

    initInterface();
    setConnections();
}

QString StartPageWidget::getName() const
{
    return "start";
}

void StartPageWidget::onStartButtonRelease()
{
    //_mainWindow->nextPage();
}

void StartPageWidget::onSwitchLanguageButtonRelease()
{

}

void StartPageWidget::initInterface()
{
    updateTexts(_ui->frame);

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
}

void StartPageWidget::setConnections()
{
    QObject::connect(_ui->startButton, &QPushButton::released, this, &StartPageWidget::onStartButtonRelease);
}

StartPageWidget::~StartPageWidget()
{
    delete _ui;
}
