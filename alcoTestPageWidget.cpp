#include <QPixmap>
#include <QPainter>
#include <QPen>
#include <QLinearGradient>
#include <QDebug>

#include "alcoTestPageWidget.h"
#include "ui_alcoTestPageWidget.h"
#include "utils.h"
#include "logger.h"

AlcoTestPageWidget::AlcoTestPageWidget(QWidget *parent)
    : Page(parent)
    , _ui(new Ui::AlcoTestPageWidget)
    , _repeatIcon(":/images/repeat.svg")
{
    _ui->setupUi(this);
}

AlcoTestPageWidget::~AlcoTestPageWidget()
{
    delete _ui;
}

void AlcoTestPageWidget::init(MainWindow *mainWindow)
{
    Page::init(mainWindow);

    _faceDetector = _mainWindow->getFaceDetector();

    ConfigManager* configManager = _mainWindow->getConfigManager();

    // fill alcolevel intervals
    _alcoLevelIntervals.insert("level1", configManager->getInterval(getName(), "level1"));
    _alcoLevelIntervals.insert("level2", configManager->getInterval(getName(), "level2"));
    _alcoLevelIntervals.insert("level3", configManager->getInterval(getName(), "level3"));
    _alcoLevelIntervals.insert("level4", configManager->getInterval(getName(), "level4"));
    _alcoLevelIntervals.insert("level5", configManager->getInterval(getName(), "level5"));
}

QString AlcoTestPageWidget::getName() const
{
    return "test";
}

QList<Transition*> AlcoTestPageWidget::getTransitions()
{
    QList<Transition*> transitions;

    // ALCOTEST -> ALCOTEST
    transitions.append(new Transition(ALCOTEST, ALCOTEST, [this](QEvent*) {
        circleCurrentPerson();
        setTimer("recognized");
    }));

    // ALCOTEST -> ALCOTEST_INACTION
    transitions.append(new Transition(ALCOTEST, ALCOTEST_INACTION, [this](QEvent*) {
        // TODO: to inaction warning
    }));

    // ALCOTEST -> DRUNKENESS_NOT_RECOGNIZED
    transitions.append(new Transition(ALCOTEST, DRUNKENESS_NOT_RECOGNIZED, [this](QEvent*) {
        circleCurrentPerson();
        setTimer("drunkenessNotRecognized");
    }));

    // ALCOTEST -> FINAL_PHOTO
    transitions.append(new Transition(ALCOTEST, FINAL_PHOTO, [this](QEvent*) {
        _mainWindow->setPage(PHOTO_PRINT_PAGE);
    }));

    // ALCOTEST_INACTION -> SPLASH_SCREEN
    transitions.append(new Transition(ALCOTEST_INACTION, SPLASH_SCREEN, [this](QEvent*) {
        _mainWindow->setPage(SPLASH_SCREEN_PAGE);
    }));

    // ALCOTEST_INACTION -> ALCOTEST
    transitions.append(new Transition(ALCOTEST_INACTION, ALCOTEST, [this](QEvent*) {
        // TODO: show regular message
    }));

    // DRUNKENESS_NOT_RECOGNIZED -> ALCOTEST
    transitions.append(new Transition(DRUNKENESS_NOT_RECOGNIZED, ALCOTEST, [this](QEvent*) {
        test(_currentPerson);
    }));

    return transitions;
}

void AlcoTestPageWidget::onEntry()
{
    qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::ALCOTEST_START, 0, 0
        , QList<double>({static_cast<double>(_faceDetector->facesCount())}));

    Machinery* machinery = _mainWindow->getMachinery();

    // connect to Machinary signals for alcotester device
    QObject::connect(_mainWindow->getMachinery(), &Machinery::error, this, &AlcoTestPageWidget::onAlcotesterError);
    QObject::connect(machinery, &Machinery::deviceHasRestarted, this, &AlcoTestPageWidget::onRestartAlcotester);

    // initial warm up
    _alcotesterWarmingUpAttemptNumber = 0;
    _alcotesterInWork = false;
    _mainWindow->getMachinery()->warmingUpAlcotester();
}

void AlcoTestPageWidget::onExit()
{
    Machinery* machinery = _mainWindow->getMachinery();

    // disconnect from Machinary signals for alcotester device
    QObject::disconnect(machinery, &Machinery::error, this, &AlcoTestPageWidget::onAlcotesterError);
    QObject::disconnect(machinery, &Machinery::deviceHasRestarted, this, &AlcoTestPageWidget::onRestartAlcotester);
}

void AlcoTestPageWidget::initInterface()
{
    updateTexts(_ui->frame);
}

void AlcoTestPageWidget::setConnections()
{
    Machinery* machinery = _mainWindow->getMachinery();

    // connect to Machinary signals for alcotester device
    // because the alcotester is used only here, you do not need to disconnect these connections when you exit the page
    QObject::connect(machinery, &Machinery::receivedAlcotesterData
                     , this, &AlcoTestPageWidget::onReceivedAlcotesterData);

    QObject::connect(machinery, &Machinery::failedToReceiveDataFromAcotester
                     , this, &AlcoTestPageWidget::onFailedToReceiveDataFromAcotester);

    QObject::connect(machinery, &Machinery::alcotesterWarmedUp
                     , this, &AlcoTestPageWidget::onWarmingUpAlcotester);
}

void AlcoTestPageWidget::onReceivedAlcotesterData(QSharedPointer<Status> status, double value)
{
    _circleState = SUCCESS;
    _lastPersonValue = value;
    _mainWindow->getSessionData().addAlcoValue(value);

    qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::ALCOTEST_SUCCESS, 0, 0
        , QList<double>({static_cast<double>(_currentPerson + 1), value}));

    _mainWindow->goToState(ALCOTEST);
}

void AlcoTestPageWidget::onFailedToReceiveDataFromAcotester(QSharedPointer<Status> status)
{
    _circleState = FAIL;

    qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::ALCOTEST_FAIL, 0, 0
        , QList<double>({static_cast<double>(_currentPerson + 1)}));

    _mainWindow->goToState(DRUNKENESS_NOT_RECOGNIZED);
}

void AlcoTestPageWidget::onAlcotesterError(QSharedPointer<Status> status)
{
    switch (status->getRequestName()) {
        case WARMING_UP_ALCOTESTER:
            _alcotesterWarmingUpAttemptNumber++;

            // try to warm up alcotester 5 times
            if (_alcotesterWarmingUpAttemptNumber == 5) {
                _mainWindow->getDevicesChecker().addDisabledDevice(status);
                _mainWindow->goToState(CRITICAL_ERROR);
            } else {
                // TODO: move time to xml
                QTimer::singleShot(2000, [this] {
                        _mainWindow->getMachinery()->warmingUpAlcotester();
                    });
            }
            break;
        case RESTART_DEVICE:
        case ACTIVATE_ALCOTESTER:
            _alcotesterWarmingUpAttemptNumber = 0;
            _alcotesterFailureNumber++;

            // try to restart alcotester 5 times
            if (_alcotesterFailureNumber == 3) {
                _mainWindow->getDevicesChecker().addDisabledDevice(status);
                _mainWindow->goToState(CRITICAL_ERROR);
            } else {
                _mainWindow->getMachinery()->restart(ALCOTESTER);
            }

            break;
        default:
            break;
    }
}

void AlcoTestPageWidget::onWarmingUpAlcotester(QSharedPointer<Status> status)
{
    if (!_alcotesterInWork) {
        // initial warm up
        _alcotesterWarmingUpAttemptNumber = 0;
        _alcotesterFailureNumber = 0;
        _alcotesterInWork = true;
        test(0);
    } else {
        // some bad shit happends and alcotester restarted
        _mainWindow->goToState(DRUNKENESS_NOT_RECOGNIZED);
    }
}

void AlcoTestPageWidget::onRestartAlcotester(QSharedPointer<Status> status)
{
    _mainWindow->getMachinery()->warmingUpAlcotester();
}

void AlcoTestPageWidget::setTimer(const QString& durationName)
{
    stopTimer();

    int timeMs = _mainWindow->getConfigManager()->getTimeDuration(getName(), durationName) * 1000;

    _timer.setInterval(timeMs);

    QObject::disconnect(&_timer, &QTimer::timeout, 0, 0);
    QObject::connect(&_timer, &QTimer::timeout, [this]{
        stopTimer();

        switch (_mainWindow->getCurrentStateName()) {
            case ALCOTEST:
                switch (_circleState) {
                    case TEST:
                        // inaction during alcotest
                        _mainWindow->goToState(ALCOTEST_INACTION);
                        break;
                    case SUCCESS:
                        test(_currentPerson + 1);
                        break;
                }
                break;
            case ALCOTEST_INACTION:
                qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::ALCOTEST_END, 0, 0
                    , QList<double>({static_cast<double>(_currentPerson)}));
                // inaction during inaction message
                _mainWindow->goToState(SPLASH_SCREEN);
                break;
            case DRUNKENESS_NOT_RECOGNIZED:
                _mainWindow->goToState(ALCOTEST);
                break;
            default:
                // TODO: handle
                break;
        }
    });

    startTimer(durationName, timeMs / 1000);
}

void AlcoTestPageWidget::test(int i)
{
    if (i == _faceDetector->facesCount()) {
        // all persons tested
        qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::ALCOTEST_END, 0, 0
            , QList<double>({static_cast<double>(i)}));

        _mainWindow->getMachinery()->coolingDownAlcotester();

        _mainWindow->goToState(FINAL_PHOTO);
        return;
    }

    _circleState = TEST;
    _currentPerson = i;

    QRect faceRect = _faceDetector->faceRects().at(_currentPerson);
    qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::PERSON_ALCOTEST_INIT_START, 0, 0
        , QList<double>({static_cast<double>(_currentPerson + 1)
                         , static_cast<double>(faceRect.top())
                         , static_cast<double>(faceRect.left())
                         , static_cast<double>(faceRect.right())
                         , static_cast<double>(faceRect.bottom())}));

    circleCurrentPerson();

    _mainWindow->getMachinery()->activateAlcotester();

    qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::PERSON_ALCOTEST_INIT_END, 0, 0
                                                                  , QList<double>({static_cast<double>(_currentPerson + 1)}));

    setTimer("inactionAlcotest");
}

void AlcoTestPageWidget::circleCurrentPerson()
{
    int w = _ui->photo->width();
    int h = _ui->photo->height();

    QPixmap notScaledImage = QPixmap::fromImage(*_mainWindow->getSessionData().getImage());

    // scale photo to QLabel "photo" size
    QPixmap image = notScaledImage.scaled(w, h, Qt::KeepAspectRatioByExpanding);
    QPixmap target(image.size());
    float scale = std::min((float)image.width() / notScaledImage.width(), (float)image.height() / notScaledImage.height());

    QRect faceRect = _faceDetector->faceRects().at(_currentPerson);

    // scale radius of main circle
    int radius = scale * std::max(faceRect.width(), faceRect.height()) / 2;
    QPoint center = faceRect.center() * scale;

    QPainter p(&target);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawPixmap(0, 0, image);

    drawPreviousPersonValues(p, scale);

    p.fillRect(0, 0, image.width(), image.height(), QColor(0, 0, 0, 0.6 * 255));

    QPen pen;
    QLinearGradient gradient(center.x() - radius, center.y() - radius
                         , center.x() + radius, center.y() + radius);


    // draw main circle
    switch (_circleState) {
        case TEST:
        case FAIL:
        {
            gradient.setColorAt(0, QColor("#ffdd33"));
            gradient.setColorAt(1, QColor("#ffb932"));

            pen = QPen(QBrush(gradient), radius * 0.2);
            break;
        }

        case SUCCESS:
        {
            const QString levelName = getAlcoLevelName();
            const QPair<QColor, QColor> gradientColors = _mainWindow->getConfigManager()->getGradient(getName(), levelName);

            gradient.setColorAt(0, gradientColors.first);
            gradient.setColorAt(1, gradientColors.second);
            pen = QPen(QBrush(gradient), radius * 0.2);
            //pen.setWidth(radius * 0.2);
            break;
        }
    }


    p.setPen(pen);

    QRegion r(QRect(faceRect.topLeft() * scale, faceRect.size() * scale), QRegion::Ellipse);
    p.setClipRegion(r);
    p.drawPixmap(0, 0, image);
    p.setClipping(false);

    p.drawEllipse(center, radius, radius);

    //draw upper circle
    QPoint upperCircleCenter(center.x() + radius * 1.2, center.y() - radius * 1.2);

    //upperCircleCenter = Utils::rotatePoint(center.x(), center.y(), -90, upperCircleCenter);

    int upperCircleRadius = radius * 0.23;

    upperCircleCenter = Utils::movePointInRect(target.rect(), upperCircleCenter, center, upperCircleRadius);


    QRect upperCircleRect(upperCircleCenter.x() - upperCircleRadius, upperCircleCenter.y() - upperCircleRadius
                           , upperCircleRadius * 2, upperCircleRadius * 2);


    switch (_circleState) {
        case TEST:
        {
            pen.setWidth(5);
            p.setPen(pen);
            p.drawEllipse(upperCircleRect);
            break;
        }

        case SUCCESS:
        {
            p.setPen(Qt::NoPen);
            p.setBrush(QBrush(gradient));
            p.drawEllipse(upperCircleRect);

            // draw alcovalue inside uppercircle
            p.setFont(Utils::getFont("Proxima Nova Rg", upperCircleRadius, 0, QFont::Bold));
            QString valueText = QString::number(_lastPersonValue);

            p.setPen(QColor(255, 255, 255));
            p.drawText(upperCircleRect, Qt::AlignCenter, valueText);

            drawSuccesText(p, radius * 0.35, QPoint(center.x(), center.y() + radius));
            break;
        }

        case FAIL:
        {
            p.drawPixmap(upperCircleRect, _repeatIcon);
        }
    }

    _ui->photo->setPixmap(target);
}

void AlcoTestPageWidget::drawPreviousPersonValues(QPainter& p, float scale)
{
    QList<double> values = _mainWindow->getSessionData().getAlcoValues();

    for (int i = 0; i < _currentPerson; i++) {
        QRect faceRect = _faceDetector->faceRects().at(i);
        int faceRectRadius = scale * std::max(faceRect.width(), faceRect.height()) / 2;
        QPoint center = faceRect.center() * scale - QPoint(0, faceRectRadius);
        int radius = faceRectRadius * 0.2;
        QRect rect(center.x() - radius, center.y() - radius
                               , radius * 2, radius * 2);

        // draw alcovalue circle
        QPen pen = QPen(QColor(255, 255, 255));
        pen.setWidth(faceRectRadius * 0.05);

        p.setPen(pen);
        p.setBrush(getAlcoLevelColor(values.at(i)));
        p.drawEllipse(center, radius, radius);

        // draw alcovalue
        p.setFont(Utils::getFont("Proxima Nova Rg", radius, 0, QFont::Bold));
        QString valueText = QString::number(values.at(i));

        p.setPen(QColor(255, 255, 255));
        p.drawText(rect, Qt::AlignCenter, valueText);
    }

    p.setBrush(Qt::NoBrush);
}

void AlcoTestPageWidget::drawSuccesText(QPainter& p, int textSize, QPoint pos)
{
    const QString levelName = getAlcoLevelName();

    if (levelName.isEmpty()) {
        // TODO: error alcolevel not found
        return;
    }

    const QString levelText = _mainWindow->getConfigManager()->getText(getName(), levelName).getText();
    const QPair<QColor, QColor> gradientColors = _mainWindow->getConfigManager()->getGradient(getName(), levelName);

    QFont textFont = Utils::getFont("Proxima Nova Rg", textSize, 0, QFont::Bold);
    QFontMetrics fm(textFont);
    int tw = fm.width(levelText);
    int th = fm.height();
    QRectF textRect(pos.x() - tw / 2, pos.y() + th / 2, tw, th);

    QLinearGradient gradient(textRect.x(), textRect.y(), textRect.right(), textRect.bottom());
    gradient.setColorAt(0, gradientColors.first);
    gradient.setColorAt(1, gradientColors.second);

    p.setPen(QPen(gradient, 0));

    p.setFont(textFont);
    p.drawText(textRect, Qt::AlignCenter, levelText);
}

QString AlcoTestPageWidget::getAlcoLevelName() const
{
    for (QString name : _alcoLevelIntervals.keys()) {
        QPair<float, float> i = _alcoLevelIntervals[name];

        if (_lastPersonValue >= i.first && _lastPersonValue < i.second) {
            return name;
        }
    }

    return "";
}

QColor AlcoTestPageWidget::getAlcoLevelColor(double value) const
{
    int num = 0;
    for (QString name : _alcoLevelIntervals.keys()) {
        QPair<float, float> i = _alcoLevelIntervals[name];

        if (value >= i.first && value < i.second) {
            // TODO: move color to config
            if (num == 0 || num == 1) {
                return QColor(113, 183, 50);
            }

            if (num == 2) {
                return QColor(255, 204, 0);
            }

            if (num == 3 || num == 4) {
                return QColor(255, 51, 51);
            }
        }

        num++;
    }

    return Qt::black;
}
