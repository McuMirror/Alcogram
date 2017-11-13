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

QList<TransitionPack> StartPageWidget::getTransitions()
{
    QList<TransitionPack> transitions;

    // START -> PREPARING_FOR_PHOTO
    Transition* transition = new Transition(TO_PREPARING_FOR_PHOTO_1_1
                                            , std::bind(&StartPageWidget::toPreparingPhoto, this));

    transitions.append(TransitionPack(transition, START, PREPARING_FOR_PHOTO));

    // START -> SPLASH_SCREEN
    transition = new Transition(TO_SPLASH_SCREEN_0_1
                                            , std::bind(&StartPageWidget::toSplashScreen, this));

    transitions.append(TransitionPack(transition, START, SPLASH_SCREEN));

    return transitions;
}

void StartPageWidget::toPreparingPhoto()
{
    _mainWindow->setPage(TAKE_PHOTO_PAGE);
}

void StartPageWidget::toSplashScreen()
{
    _mainWindow->setPage(SPLASH_SCREEN_PAGE);
}

void StartPageWidget::onEntry()
{
    int timeMs = _mainWindow->getConfigManager()->getTimeDuration(getName(), "inaction") * 1000;

    timer.setInterval(timeMs);

    QObject::disconnect(&timer, &QTimer::timeout, 0, 0);
    QObject::connect(&timer, &QTimer::timeout, [=]{
        _mainWindow->postEvent(new Event(TO_SPLASH_SCREEN_0_1, START));
    });

    timer.start();
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
    QObject::connect(_ui->startButton, &QPushButton::released, [=]{
        _mainWindow->postEvent(new Event(TO_PREPARING_FOR_PHOTO_1_1, START));
    });
}

StartPageWidget::~StartPageWidget()
{
    delete _ui;
}
