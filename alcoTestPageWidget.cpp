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

    _alcotester = _mainWindow->getDeviceManager()->getAlcotesterDevice();
    _camera = _mainWindow->getDeviceManager()->getCameraDevice();
    _faceDetector = _mainWindow->getFaceDetector();

    ConfigManager* configManager = _mainWindow->getConfigManager();

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
        , QList<double>({_faceDetector->facesNumber()}));

    test(0);
}

void AlcoTestPageWidget::initInterface()
{
    updateTexts(_ui->frame);
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
                        _mainWindow->goToState(ALCOTEST_INACTION);
                        break;
                    case SUCCESS:
                        test(_currentPerson + 1);
                        break;
                }
                break;
            case ALCOTEST_INACTION:
                qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::ALCOTEST_END, 0, 0
                    , QList<double>({_currentPerson}));

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
    if (i == _faceDetector->facesNumber()) {
        qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::ALCOTEST_END, 0, 0
            , QList<double>({i}));

        _mainWindow->goToState(FINAL_PHOTO);
        return;
    }

    _circleState = TEST;
    _currentPerson = i;

    QRect faceRect = _faceDetector->faceRects().at(_currentPerson);
    qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::PERSON_ALCOTEST_INIT_START, 0, 0
        , QList<double>({_currentPerson + 1, faceRect.top(), faceRect.left(), faceRect.right(), faceRect.bottom()}));

    circleCurrentPerson();

    _alcotester->test([=](int statusCode, double value) {
        switch (statusCode) {
            case OK:
                _circleState = SUCCESS;
                _lastPersonValue = value;

                qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::ALCOTEST_SUCCESS, 0, 0
                    , QList<double>({_currentPerson + 1, value}));

                _mainWindow->goToState(ALCOTEST);
                break;
            case NOT_RECOGNIZED:
                _circleState = FAIL;

                qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::ALCOTEST_FAIL, 0, 0
                    , QList<double>({_currentPerson + 1}));

                _mainWindow->goToState(DRUNKENESS_NOT_RECOGNIZED);
                break;
            case DEVICE_ERROR:
                break;
        }
    });

    qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::PERSON_ALCOTEST_INIT_END, 0, 0
                                                                  , QList<double>({_currentPerson + 1}));

    setTimer("inactionAlcotest");
}

void AlcoTestPageWidget::circleCurrentPerson()
{
    int w = _ui->photo->width();
    int h = _ui->photo->height();

    QPixmap notScaledImage = QPixmap::fromImage(_camera->getCapturedImage());

    // scale photo to QLabel "photo" size
    QPixmap image = notScaledImage.scaled(w, h, Qt::KeepAspectRatioByExpanding);
    float scale = std::min((float)image.width() / notScaledImage.width(), (float)image.height() / notScaledImage.height());

    QRect faceRect = _faceDetector->faceRects().at(_currentPerson);

    // scale radius of main circle
    int radius = scale * std::max(faceRect.width(), faceRect.height()) / 2;
    QPoint center = faceRect.center() * scale;

    QPainter p(&image);
    p.setRenderHint(QPainter::Antialiasing);

    QPen pen;

    // draw main circle
    switch (_circleState) {
        case TEST:
        case FAIL:
        {
            QLinearGradient gradient(center.x() - radius, center.y() - radius
                                     , center.x() + radius, center.y() + radius);
            gradient.setColorAt(0, QColor("#ffdd33"));
            gradient.setColorAt(1, QColor("#ffb932"));

            pen = QPen(QBrush(gradient), radius * 0.2);
            break;
        }

        case SUCCESS:
        {
            pen = QPen(QColor("#71b732"));
            pen.setWidth(radius * 0.2);
            break;
        }
    }


    p.setPen(pen);

    p.drawEllipse(center, radius, radius);

    //draw upper circle
    QPoint upperCircleCenter(center.x() + radius * 1.2, center.y() - radius * 1.2);
    int upperCircleRadius = radius * 0.23;
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
            p.setBrush(QBrush(QColor("#71b732")));
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

    _ui->photo->setPixmap(image);
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
