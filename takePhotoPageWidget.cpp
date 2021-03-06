#include <QGraphicsDropShadowEffect>
#include <QDateTime>
#include <QPixmap>
#include <QRgb>
#include <QPainter>
#include <QColor>
#include <QThread>
#include <QDebug>
#include <QFontMetrics>

//#include <opencv2\imgproc.hpp>

#include "takePhotoPageWidget.h"
#include "ui_takePhotoPageWidget.h"
#include "utils.h"
#include "deviceInterfaces/cameraInterface.h"
#include "faceDetector.h"
#include "logger.h"


TakePhotoPageWidget::TakePhotoPageWidget(QWidget *parent)
    : Page(parent)
    , _ui(new Ui::TakePhotoPageWidget)
    , _bottomPanelPageNumbers(initBottomPanelPageNumbers())
    , _mainPanelPageNumbers(initMainPanelPageNumbers())
    , _imageProcessingThread(this)
{
    _ui->setupUi(this);
}

TakePhotoPageWidget::~TakePhotoPageWidget()
{
    _imageProcessingThread.quit();

    delete _ui;
}

void TakePhotoPageWidget::onNextFrameReceived(QSharedPointer<QImage> image, QSharedPointer<Status> status)
{
    if (!_isImageHandling) {
        _isImageHandling = true;
        _cameraImageHandler.setImageToProcess(image);
        emit startImageHandle();
    }
}

void TakePhotoPageWidget::onReceivedCapturedImage(QSharedPointer<QImage> image, QSharedPointer<Status> status)
{
    if (_faceDetector->facesCount() > 0) {
        _mainWindow->getSessionData().setImage(image);
        _mainWindow->goToState(PHOTO_CONFIRMATION);
    } else {
        setErrorSubPage("facesNotFoundMsg", "facesNotFoundMsg");
    }
}

void TakePhotoPageWidget::onCameraRestart(QSharedPointer<Status> status)
{
    StateName stateName = _mainWindow->getCurrentStateName();

    if (stateName == PHOTO_TIMER) {
        _mainWindow->goToState(PREPARING_FOR_PHOTO);
    } else {
        setSubPage(stateName);
    }

    _isImageCapturing = false;
    _mainWindow->getMachinery()->getImage();
}

void TakePhotoPageWidget::onCameraError(QSharedPointer<Status> status)
{
    if (status->getRequestName() == RESTART_DEVICE) {
        _mainWindow->getDevicesChecker().addDisabledDevice(status);
        _mainWindow->goToState(CRITICAL_ERROR);
    }


    if (status->getRequestName() == GET_IMAGE || status->getRequestName() == TAKE_IMAGE) {
        SessionData* sessionData = &_mainWindow->getSessionData();

        sessionData->cameraGetImageTimeout();

        if (sessionData->getCameraGetImageTimeoutNumber() == 2) {
            _mainWindow->getDevicesChecker().addDisabledDevice(status);
            _mainWindow->goToState(CRITICAL_ERROR);
        } else {
            setErrorSubPage("cameraNoResponceMsg");
            _mainWindow->getMachinery()->restart(CAMERA);
        }
    }
}

void TakePhotoPageWidget::updateCameraOutput(QPixmap processedImage)
{
    int w = _ui->cameraOutput->width();
    int h = _ui->cameraOutput->height();
    double cutSize = _mainWindow->getConfigManager()->getSize(getName(), "liveViewCutSize");
    int x = (int) (cutSize * processedImage.width());
    int y = (int) (cutSize * processedImage.height());

    QRect rect(x, y, processedImage.width() - x, processedImage.height() - y);
    QPixmap f = processedImage.copy(rect);

    _ui->cameraOutput->setPixmap(f.scaled(w, h, Qt::KeepAspectRatioByExpanding));
    _ui->cameraOutput->update();

    _isImageHandling = false;

    // TODO: weak point
    if (!_isImageCapturing) {
        _mainWindow->getMachinery()->getImage();
    }
}

void TakePhotoPageWidget::init(MainWindowInterface* mainWindow) {
    Page::init(mainWindow);

    _faceDetector = _mainWindow->getFaceDetector();

    _cameraImageHandler.setFaceDetector(_faceDetector);
    _cameraImageHandler.moveToThread(&_imageProcessingThread);

    QObject::connect(&_cameraImageHandler, &CameraImageHandler::processed, this
                     , &TakePhotoPageWidget::updateCameraOutput, Qt::QueuedConnection);

    QObject::connect(this, &TakePhotoPageWidget::startImageHandle
                     , &_cameraImageHandler, &CameraImageHandler::process, Qt::QueuedConnection);

    _imageProcessingThread.start();

    QObject::connect(mainWindow->getMachinery(), &Machinery::receivedNextFrame
                     , this, &TakePhotoPageWidget::onNextFrameReceived);

    QObject::connect(mainWindow->getMachinery(), &Machinery::receivedCapturedImage
                     , this, &TakePhotoPageWidget::onReceivedCapturedImage);
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
        stopTimer();
        _mainWindow->setPage(SPLASH_SCREEN_PAGE);
    }));

    // PREPARING_FOR_PHOTO -> START
    transitions.append(new Transition(PREPARING_FOR_PHOTO, START, [=](QEvent*) {
        stopTimer();
        _mainWindow->setPage(START_PAGE);
    }));

    // PREPARING_FOR_PHOTO -> PHOTO_TIMER
    transitions.append(new Transition(PREPARING_FOR_PHOTO, PHOTO_TIMER, [=](QEvent*) {
        setSubPage(PHOTO_TIMER);
        setPhotoTimer();
    }));

    // PHOTO_TIMER -> PREPARING_FOR_PHOTO
    transitions.append(new Transition(PHOTO_TIMER, PREPARING_FOR_PHOTO, [=] (QEvent*) {
        setInactionTimer("inactionPreparingPhoto");
        setSubPage(PREPARING_FOR_PHOTO);
    }));

    // PHOTO_TIMER -> PHOTO_CONFIRMATION
    transitions.append(new Transition(PHOTO_TIMER, PHOTO_CONFIRMATION, [=] (QEvent*) {
        setSubPage(PHOTO_CONFIRMATION);
        int w = _ui->takenPhoto->width();
        int h = _ui->takenPhoto->height();

        _ui->takenPhoto->setPixmap(QPixmap::fromImage(*_mainWindow->getSessionData().getImage())
                                   .scaled(w, h, Qt::KeepAspectRatioByExpanding));
        setInactionTimer("inactionPhotoConfirmation");
    }));

    // PHOTO_CONFIRMATION -> SPLASH_SCREEN
    transitions.append(new Transition(PHOTO_CONFIRMATION, SPLASH_SCREEN, [=](QEvent*) {
        stopTimer();
        _mainWindow->setPage(SPLASH_SCREEN_PAGE);
    }));

    // PHOTO_CONFIRMATION -> PREPARING_FOR_PHOTO
    transitions.append(new Transition(PHOTO_CONFIRMATION, PREPARING_FOR_PHOTO, [=] (QEvent*) {
        setInactionTimer("inactionPreparingPhoto");
        //_camera->reset();
        //_camera->setImageCaptureCallback(_cameraStreamCallback);
        _mainWindow->getMachinery()->getImage();
        setSubPage(PREPARING_FOR_PHOTO);
    }));

    //PHOTO_CONFIRMATION -> PAY
    transitions.append(new Transition(PHOTO_CONFIRMATION, PAY, [=](QEvent*) {
        stopTimer();
        _mainWindow->setPage(PAY_PAGE);
    }));

    return transitions;
}

void TakePhotoPageWidget::onEntry()
{
    setSubPage(PREPARING_FOR_PHOTO);
    setInactionTimer("inactionPreparingPhoto");

    QObject::connect(_mainWindow->getMachinery(), &Machinery::error
                     , this, &TakePhotoPageWidget::onCameraError);

    QObject::connect(_mainWindow->getMachinery(), &Machinery::deviceHasRestarted
                     , this, &TakePhotoPageWidget::onCameraRestart);

    _isImageCapturing = false;
    _mainWindow->getMachinery()->getImage();
}

void TakePhotoPageWidget::onExit()
{
    QObject::disconnect(_mainWindow->getMachinery(), &Machinery::error
                     , this, &TakePhotoPageWidget::onCameraError);

    QObject::disconnect(_mainWindow->getMachinery(), &Machinery::deviceHasRestarted
                     , this, &TakePhotoPageWidget::onCameraRestart);
}

void TakePhotoPageWidget::initInterface()
{
    updateTexts(_ui->frame);

    //timer font
    _ui->timer->setFont(Utils::getFont("Proxima Nova Rg", 47, 2, QFont::Bold));

    //takenPhotoFrame shadow
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(128);
    effect->setColor(QColor(0, 0, 0, 127));
    effect->setOffset(0, 42);

    _ui->takenPhotoFrame->setGraphicsEffect(effect);
}

void TakePhotoPageWidget::setConnections()
{
    QObject::connect(_ui->cancelButton, &QPushButton::released, [this] {
        qDebug().noquote() << Logger::instance()->buildUserActionLog(Logger::BUTTON_RELEASE, Logger::BUTTON
            , _ui->cancelButton->objectName());

        StateName targetState;

        switch (_mainWindow->getCurrentStateName()) {
        case PREPARING_FOR_PHOTO:
            targetState = START;
            break;

        case PHOTO_CONFIRMATION:
        case PHOTO_TIMER:
            _isImageCapturing = false;
            targetState = PREPARING_FOR_PHOTO;
            break;

        default:
            // TODO: logging, wrong state
            return;
        }

        qDebug().noquote() << Logger::instance()->buildUserActionLog(Logger::BUTTON_RELEASE, Logger::BUTTON
            , _ui->cancelButton->objectName(), QList<double>()
            , QStringList({Utils::getStateNameNumber(targetState)}));

        _mainWindow->goToState(targetState);
    });

    QObject::connect(_ui->takePhotoButton, &QPushButton::released, [this] {
        qDebug().noquote() << Logger::instance()->buildUserActionLog(Logger::BUTTON_RELEASE, Logger::BUTTON
            , _ui->takePhotoButton->objectName());

        _mainWindow->goToState(PHOTO_TIMER);
    });

    QObject::connect(_ui->retakePhotoButton, &QPushButton::released, [this] {
        qDebug().noquote() << Logger::instance()->buildUserActionLog(Logger::BUTTON_RELEASE, Logger::BUTTON
            , _ui->retakePhotoButton->objectName());

        _isImageCapturing = false;
        _mainWindow->getSessionData().removeImage();
        _mainWindow->goToState(PREPARING_FOR_PHOTO);
    });

    QObject::connect(_ui->continueButton, &QPushButton::released, [this] {
        qDebug().noquote() << Logger::instance()->buildUserActionLog(Logger::BUTTON_RELEASE, Logger::BUTTON
            , _ui->continueButton->objectName());

        _mainWindow->goToState(PAY);
    });
}

void TakePhotoPageWidget::setPhotoTimer()
{
    stopTimer();

    // update timer label with start time
    _timerTimeLeft = _mainWindow->getConfigManager()->getTimeDuration(getName(), "timer") * 1000;
    QString timerText = QDateTime::fromMSecsSinceEpoch(_timerTimeLeft).toString("mm:ss:zzz");
    timerText.chop(1);
    _ui->timer->setText(timerText);

    _timer.setInterval(50); // each 50 ms

    QObject::disconnect(&_timer, &QTimer::timeout, 0, 0);
    QObject::connect(&_timer, &QTimer::timeout, [this]{
        _timerTimeLeft -= 50;

        // update timer label
        QString timerText = QDateTime::fromMSecsSinceEpoch(_timerTimeLeft).toString("mm:ss:zzz");
        timerText.chop(1);

        _ui->timer->setText(timerText);
        _ui->timer->update();

        if (_timerTimeLeft <= 0) {
            stopTimer();
            _isImageCapturing = true;
            _mainWindow->getMachinery()->stopGetImage();
            _mainWindow->getMachinery()->takeImage();
        };
    });

    startTimer("timer", _timerTimeLeft / 1000);
}

void TakePhotoPageWidget::setInactionTimer(const QString& durationName)
{   
    stopTimer();

    int timeMs = _mainWindow->getConfigManager()->getTimeDuration(getName(), durationName) * 1000;

    _timer.setInterval(timeMs);

    QObject::disconnect(&_timer, &QTimer::timeout, 0, 0);
    QObject::connect(&_timer, &QTimer::timeout, [=]{
        stopTimer();

        _mainWindow->goToState(SPLASH_SCREEN);
    });

    startTimer(durationName, timeMs / 1000);
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

void TakePhotoPageWidget::setErrorSubPage(const QString& textName, const QString& durationName)
{
    if (!textName.isEmpty()) {
        Text t = _mainWindow->getConfigManager()->getText(getName(), textName);

        if (t.getSize() != 0) {
            // plain text
            _ui->errorMsg->setFont(Utils::getFont(t.getFontFamily(), t.getSize()
                                      , t.getSpacing(), t.getWeight()));
        } else {
            // html text
            _ui->errorMsg->setTextFormat(Qt::RichText);
        }

        QFontMetrics fm(_ui->errorMsg->font());

        _ui->errorMsg->setFixedHeight(fm.height());
        _ui->errorMsg->setText(t.getText());
    }

    _ui->bottomPanel->setCurrentIndex(ERROR_SUBPAGE_BOTTOM);
    _ui->mainPanel->setCurrentIndex(ERROR_SUBPAGE_MAIN);

    if (!durationName.isEmpty()) {
        stopTimer();

        int timeMs = _mainWindow->getConfigManager()->getTimeDuration(getName(), durationName) * 1000;

        _timer.setInterval(timeMs);

        QObject::disconnect(&_timer, &QTimer::timeout, 0, 0);
        QObject::connect(&_timer, &QTimer::timeout, [=]{
            stopTimer();
            _isImageCapturing = false;
            _mainWindow->getMachinery()->getImage();
            _mainWindow->goToState(PREPARING_FOR_PHOTO);
        });

        startTimer(durationName, timeMs / 1000);
    }
}
