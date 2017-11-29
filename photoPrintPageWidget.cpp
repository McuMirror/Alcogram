#include <QGraphicsDropShadowEffect>
#include <QPixmap>
#include <QTimer>
#include <QDebug>

#include "photoPrintPageWidget.h"
#include "ui_photoPrintPageWidget.h"
#include "utils.h"
#include "logger.h"

PhotoPrintPageWidget::PhotoPrintPageWidget(QWidget *parent) :
    Page(parent),
    _ui(new Ui::PhotoPrintPageWidget)
{
    _ui->setupUi(this);
}

PhotoPrintPageWidget::~PhotoPrintPageWidget()
{
    delete _ui;
}

void PhotoPrintPageWidget::init(MainWindow* mainWindow)
{
    Page::init(mainWindow);

    DeviceManager* deviceManager = _mainWindow->getDeviceManager();

    _printer = deviceManager->getPrinterDevice();
    _camera = deviceManager->getCameraDevice();
    _alcotester = deviceManager->getAlcotesterDevice();
    _faceDetector = _mainWindow->getFaceDetector();
}

QString PhotoPrintPageWidget::getName() const
{
    return "print";
}

QList<Transition*> PhotoPrintPageWidget::getTransitions()
{
    QList<Transition*> transitions;

    // FINAL_PHOTO -> PHOTO_PRINT
    transitions.append(new Transition(FINAL_PHOTO, PHOTO_PRINT, [this](QEvent*) {
        _ui->printPages->setCurrentIndex(1);

        _ui->progressBar->setProgress(0);

        qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::PRINTING_PHOTOS_START, 0, 0
            , QList<double>({_faceDetector->facesNumber()}));

        // TODO: final photo
        _printer->print(_camera->getCapturedImage(), /*_faceDetector->facesNumber()*/ 6, [=](int statusCode, int printed){
            switch (statusCode) {
                case OK:
                    qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::PHOTO_PRINT_SUCCESS
                        , 0, 0, QList<double>({printed, _faceDetector->facesNumber()}));

                    _ui->progressBar->setProgress(100 * printed / 6/*_faceDetector->facesNumber()*/);
                    break;
               case DEVICE_ERROR:
                    qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::PHOTO_PRINT_FAIL
                        , 1, 0, QList<double>());
                    // TODO: handle this case
                    break;
               case PRINT_COMPLETE:
                    qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::PRINTING_PHOTOS_END
                                   , 0, 0, QList<double>({printed, _faceDetector->facesNumber()}));

                    _ui->progressBar->setProgress(100);

                    setTimer("photoPrint");
                    break;
            }
        });
    }));

    // PHOTO_PRINT -> START
    transitions.append(new Transition(PHOTO_PRINT, START, [this](QEvent*) {
        _mainWindow->setPage(START_PAGE);
    }));

    return transitions;
}

void PhotoPrintPageWidget::onEntry()
{
    _ui->printPages->setCurrentIndex(0);

    QPixmap image = QPixmap::fromImage(_camera->getCapturedImage());

    // TODO: generate final photo

    int w = _ui->finalPhoto->width();
    int h = _ui->finalPhoto->height();

    _ui->finalPhoto->setPixmap(image.scaled(w, h, Qt::KeepAspectRatioByExpanding));

    w = _ui->previewPhoto->width();
    h = _ui->previewPhoto->height();

    _ui->previewPhoto->setPixmap(image.scaled(w, h, Qt::KeepAspectRatioByExpanding));

    setTimer("finalPhoto");
}

void PhotoPrintPageWidget::initInterface()
{
    updateTexts(_ui->frame);

    //preview shadow
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);

    effect->setBlurRadius(128);
    effect->setColor(QColor(0, 0, 0, 127));
    effect->setOffset(0, 42);

    _ui->preview->setGraphicsEffect(effect);
}

void PhotoPrintPageWidget::setTimer(const QString& durationName)
{
    stopTimer();

    int timeMs = _mainWindow->getConfigManager()->getTimeDuration(getName(), durationName) * 1000;

    _timer.setInterval(timeMs);

    QObject::disconnect(&_timer, &QTimer::timeout, 0, 0);
    QObject::connect(&_timer, &QTimer::timeout, [this]{
        stopTimer();

        switch (_mainWindow->getCurrentStateName()) {
            case FINAL_PHOTO:
                _mainWindow->goToState(PHOTO_PRINT);
                break;
            case PHOTO_PRINT:
                _mainWindow->goToState(START);
                break;
        }
    });

    startTimer(durationName, timeMs / 1000);
}
