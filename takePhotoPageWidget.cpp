#include <QGraphicsDropShadowEffect>
#include <QDateTime>

#include "takePhotoPageWidget.h"
#include "ui_takePhotoPageWidget.h"
#include "utils.h"

TakePhotoPageWidget::TakePhotoPageWidget(QWidget *parent)
    : Page(parent)
    , _ui(new Ui::TakePhotoPageWidget)
    , _bottomPanelPageNumbers(initBottomPanelPageNumbers())
    , _mainPanelPageNumbers(initMainPanelPageNumbers())
{
    _ui->setupUi(this);
}

TakePhotoPageWidget::~TakePhotoPageWidget()
{
    delete _ui;
}

QString TakePhotoPageWidget::getName() const
{
    return "take";
}

QList<Transition*> TakePhotoPageWidget::getTransitions()
{
    QList<Transition*> transitions;

    // PREPARING_FOR_PHOTO -> SPLASH_SCREEN
    transitions.append(new Transition(PREPARING_FOR_PHOTO, SPLASH_SCREEN, [=](QEvent*) {
        _mainWindow->setPage(SPLASH_SCREEN_PAGE);
    }));

    // PREPARING_FOR_PHOTO -> START
    transitions.append(new Transition(PREPARING_FOR_PHOTO, START, [=](QEvent*) {
        _timer.stop();
        _mainWindow->setPage(START_PAGE);
    }));

    // PREPARING_FOR_PHOTO -> PHOTO_TIMER
    transitions.append(new Transition(PREPARING_FOR_PHOTO, PHOTO_TIMER, [=](QEvent*) {
        _timer.stop();
        setSubPage(PHOTO_TIMER);
        setPhotoTimer();
    }));

    // PHOTO_TIMER -> PREPARING_FOR_PHOTO
    transitions.append(new Transition(PHOTO_TIMER, PREPARING_FOR_PHOTO, [=] (QEvent*) {
        _timer.stop();
        setSubPage(PREPARING_FOR_PHOTO);
    }));

    // PHOTO_TIMER -> PHOTO_CONFIRMATION
    transitions.append(new Transition(PHOTO_TIMER, PHOTO_CONFIRMATION, [=] (QEvent*) {
        // TODO: take photo
        setSubPage(PHOTO_CONFIRMATION);
        setInactionTimer("inactionPhotoConfirmation");
    }));

    // PHOTO_CONFIRMATION -> SPLASH_SCREEN
    transitions.append(new Transition(PHOTO_CONFIRMATION, SPLASH_SCREEN, [=](QEvent*) {
        _mainWindow->setPage(SPLASH_SCREEN_PAGE);
    }));

    // PHOTO_CONFIRMATION -> PREPARING_FOR_PHOTO
    transitions.append(new Transition(PHOTO_CONFIRMATION, PREPARING_FOR_PHOTO, [=] (QEvent*) {
        _timer.stop();
        setSubPage(PREPARING_FOR_PHOTO);
    }));

    //PHOTO_CONFIRMATION -> PAY
    transitions.append(new Transition(PHOTO_CONFIRMATION, PAY, [=](QEvent*) {
        _mainWindow->setPage(PAY_PAGE);
    }));

    return transitions;
}

void TakePhotoPageWidget::onEntry()
{
    setSubPage(PREPARING_FOR_PHOTO);
    setInactionTimer("inactionPreparingPhoto");
}

void TakePhotoPageWidget::initInterface()
{
    updateTexts(_ui->frame);

    //timer font
    _ui->timer->setFont(Utils::getFont("Proxima Nova Rg", 47, 2, QFont::Bold));

    //takenPhoto shadow
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(128);
    effect->setColor(QColor(0, 0, 0, 127));
    effect->setOffset(0, 42);

    _ui->takenPhoto->setGraphicsEffect(effect);
}

void TakePhotoPageWidget::setConnections()
{
    QObject::connect(_ui->cancelButton, &QPushButton::released, [this] {
        StateName targetState;

        switch (_mainWindow->getCurrentStateName()) {
        case PREPARING_FOR_PHOTO:
            targetState = START;
            break;

        case PHOTO_CONFIRMATION:
        case PHOTO_TIMER:
            targetState = PREPARING_FOR_PHOTO;
            break;

        default:
            // TODO: logging, wrong state
            return;
        }

        _mainWindow->postEvent(targetState);
    });

    QObject::connect(_ui->takePhotoButton, &QPushButton::released, [this] {
        _mainWindow->postEvent(PHOTO_TIMER);
    });

    QObject::connect(_ui->retakePhotoButton, &QPushButton::released, [this] {
        _mainWindow->postEvent(PREPARING_FOR_PHOTO);
    });

    QObject::connect(_ui->continueButton, &QPushButton::released, [this] {
        _mainWindow->postEvent(PAY);
    });
}

void TakePhotoPageWidget::setPhotoTimer()
{
    _timerTimeLeft = _mainWindow->getConfigManager()->getTimeDuration(getName(), "timer") * 1000;
    QString timerText = QDateTime::fromMSecsSinceEpoch(_timerTimeLeft).toString("mm:ss:zzz");
    timerText.chop(1);
    _ui->timer->setText(timerText);

    _timer.stop();
    _timer.setInterval(10); // each 10 ms

    QObject::disconnect(&_timer, &QTimer::timeout, 0, 0);
    QObject::connect(&_timer, &QTimer::timeout, [this]{
        _timerTimeLeft -= 10;

        // update timer label
        QString timerText = QDateTime::fromMSecsSinceEpoch(_timerTimeLeft).toString("mm:ss:zzz");
        timerText.chop(1);
        _ui->timer->setText(timerText);

        if (_timerTimeLeft < 0) {
            _timer.stop();
            _mainWindow->postEvent(PHOTO_CONFIRMATION);
        };
    });

    _timer.start();
}

void TakePhotoPageWidget::setInactionTimer(const QString& durationName)
{
    int timeMs = _mainWindow->getConfigManager()->getTimeDuration(getName(), durationName) * 1000;

    _timer.setInterval(timeMs);

    QObject::disconnect(&_timer, &QTimer::timeout, 0, 0);
    QObject::connect(&_timer, &QTimer::timeout, [=]{
        _mainWindow->postEvent(SPLASH_SCREEN);
        _timer.stop();
    });

    _timer.start();
}

QMap<StateName, int> TakePhotoPageWidget::initBottomPanelPageNumbers() const
{
    QMap<StateName, int> map;

    map.insert(PREPARING_FOR_PHOTO, 0);
    map.insert(PHOTO_TIMER, 1);
    map.insert(PHOTO_CONFIRMATION, 2);

    return map;
}

QMap<StateName, int> TakePhotoPageWidget::initMainPanelPageNumbers() const
{
    QMap<StateName, int> map;

    map.insert(PREPARING_FOR_PHOTO, 0);
    map.insert(PHOTO_TIMER, 0);
    map.insert(PHOTO_CONFIRMATION, 1);

    return map;
}

void TakePhotoPageWidget::setSubPage(StateName stateName)
{
    int bottomIndex = _bottomPanelPageNumbers[stateName];
    int mainIndex = _mainPanelPageNumbers[stateName];

    _ui->bottomPanel->setCurrentIndex(bottomIndex);
    _ui->mainPanel->setCurrentIndex(mainIndex);
}
