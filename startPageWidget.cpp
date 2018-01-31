#include <QGraphicsDropShadowEffect>
#include <QFontDatabase>
#include <QDebug>
#include <QTimer>

#include "startPageWidget.h"
#include "ui_startPageWidget.h"
#include "utils.h"
#include "configManager.h"
#include "logger.h"

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
    _mainWindow->getSessionData().reset();
    _checkConnectionAttempt = 0;
    _connectedToDeviceChecker = false;
    checkDevicesConnection();

    // inaction timer
    int timeMs = _mainWindow->getConfigManager()->getTimeDuration(getName(), "inaction") * 1000;

    _timer.setInterval(timeMs);

    QObject::disconnect(&_timer, &QTimer::timeout, 0, 0);
    QObject::connect(&_timer, &QTimer::timeout, [=]{
        stopTimer();

        _mainWindow->goToState(SPLASH_SCREEN);
    });

    startTimer("inaction", timeMs / 1000);
}

void StartPageWidget::onSwitchLanguageButtonRelease()
{
    qDebug().noquote() << Logger::instance()->buildUserActionLog(Logger::BUTTON_RELEASE
                                                                 , Logger::BUTTON, _ui->switchLanguageButton->objectName());

    _mainWindow->switchLanguage();
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
        qDebug().noquote() << Logger::instance()->buildUserActionLog(Logger::BUTTON_RELEASE
            , Logger::BUTTON, _ui->startButton->objectName());

        _timer.stop();

        qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::USER_TIMER_STOP, Logger::NONE, "inaction");

        _mainWindow->goToState(PREPARING_FOR_PHOTO);
    });

    QObject::connect(_ui->switchLanguageButton, &QPushButton::released, this
                     , &StartPageWidget::onSwitchLanguageButtonRelease);

}

void StartPageWidget::onDevicesConnected()
{
    // TODO: logging
    if (_checkConnectionAttempt == 2) {
        _ui->pages->setCurrentIndex(MAIN_SUBPAGE);
    }

    disconnectFromDevicesChecker();
}

void StartPageWidget::onSomeDevicesNotConnected()
{
    // TODO: logging
    if (_checkConnectionAttempt != 2) {
       _ui->pages->setCurrentIndex(ERROR_SUBPAGE);

       int timeMs = _mainWindow->getConfigManager()
               ->getTimeDuration(getName(), "delayBwCheckConnectionAttempts") * 1000;

       QTimer::singleShot(timeMs, [this] {
               checkDevicesConnection();
           });
    } else {
        _mainWindow->goToState(CRITICAL_ERROR);
    }
}

void StartPageWidget::checkDevicesConnection()
{
    _checkConnectionAttempt++;

    DevicesChecker* devicesChecker = &_mainWindow->getDevicesChecker();

    if (!_connectedToDeviceChecker) {
        QObject::connect(devicesChecker, &DevicesChecker::devicesConnected, this, &StartPageWidget::onDevicesConnected);
        QObject::connect(devicesChecker, &DevicesChecker::someDevicesNotConnected
                        , this, &StartPageWidget::onSomeDevicesNotConnected);

        _connectedToDeviceChecker = true;
    }

    devicesChecker->checkDevicesConnection();
}

void StartPageWidget::disconnectFromDevicesChecker()
{
    DevicesChecker* devicesChecker = &_mainWindow->getDevicesChecker();

    QObject::disconnect(devicesChecker, &DevicesChecker::devicesConnected, 0, 0);
    QObject::disconnect(devicesChecker, &DevicesChecker::someDevicesNotConnected, 0, 0);

    _connectedToDeviceChecker = false;

}
