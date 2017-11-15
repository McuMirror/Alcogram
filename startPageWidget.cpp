#include <QGraphicsDropShadowEffect>
#include <QFontDatabase>

#include "startPageWidget.h"
#include "ui_startPageWidget.h"
#include "utils.h"
#include "configManager.h"

StartPageWidget::StartPageWidget(QWidget *parent)
    : Page(parent)
    , _ui(new Ui::StartPageWidget)
{
    _ui->setupUi(this);
}

StartPageWidget::~StartPageWidget()
{
    delete _ui;
}

QString StartPageWidget::getName() const
{
    return "start";
}

QList<Transition*> StartPageWidget::getTransitions()
{
    QList<Transition*> transitions;

    // START -> PREPARING_FOR_PHOTO
    transitions.append(new Transition(START, PREPARING_FOR_PHOTO, [=](QEvent*) {
        _mainWindow->setPage(TAKE_PHOTO_PAGE);
    }));

    // START -> SPLASH_SCREEN
    transitions.append(new Transition(START, SPLASH_SCREEN, [=] (QEvent*) {
        _mainWindow->setPage(SPLASH_SCREEN_PAGE);
    }));

    return transitions;
}

void StartPageWidget::onEntry()
{
    // inaction timer
    int timeMs = _mainWindow->getConfigManager()->getTimeDuration(getName(), "inaction") * 1000;

    _timer.setInterval(timeMs);

    QObject::disconnect(&_timer, &QTimer::timeout, 0, 0);
    QObject::connect(&_timer, &QTimer::timeout, [=]{
        _timer.stop();
        _mainWindow->goToState(SPLASH_SCREEN);
    });

    _timer.start();
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
        _timer.stop();
        _mainWindow->goToState(PREPARING_FOR_PHOTO);
    });

    QObject::connect(_ui->switchLanguageButton, &QPushButton::released, this
                     , &StartPageWidget::onSwitchLanguageButtonRelease);
}
