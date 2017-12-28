#include <QGraphicsDropShadowEffect>
#include <QPixmap>
#include <QPainter>
#include <QPolygon>
#include <QPainterPath>
#include <QPen>
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

    ConfigManager* configManager = _mainWindow->getConfigManager();

    // TODO: get rid of "test"
    _alcoLevelIntervals.insert("level1", configManager->getInterval("test", "level1"));
    _alcoLevelIntervals.insert("level2", configManager->getInterval("test", "level2"));
    _alcoLevelIntervals.insert("level3", configManager->getInterval("test", "level3"));
    _alcoLevelIntervals.insert("level4", configManager->getInterval("test", "level4"));
    _alcoLevelIntervals.insert("level5", configManager->getInterval("test", "level5"));
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
        _printer->print(_camera->getCapturedImage(), _faceDetector->facesNumber(), [=](int statusCode, int printed){
            switch (statusCode) {
                case OK:
                    qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::PHOTO_PRINT_SUCCESS
                        , 0, 0, QList<double>({printed, _faceDetector->facesNumber()}));

                    _ui->progressBar->setProgress(100 * printed / _faceDetector->facesNumber());
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

    //QPixmap image = generateFinalPhoto();//QPixmap::fromImage(_camera->getCapturedImage());

    // TODO: generate final photo

    int w = _ui->finalPhoto->width();
    int h = _ui->finalPhoto->height();

    _ui->finalPhoto->setPixmap(generateFinalPhoto(w, h));//image.scaled(w, h, Qt::KeepAspectRatioByExpanding));

    w = _ui->previewPhoto->width();
    h = _ui->previewPhoto->height();

    _ui->previewPhoto->setPixmap(generateFinalPhoto(w, h));//image.scaled(w, h, Qt::KeepAspectRatioByExpanding));

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

QPixmap PhotoPrintPageWidget::generateFinalPhoto(int w, int h)
{
    QImage notScaledImage = _camera->getCapturedImage();
    QPixmap image = QPixmap::fromImage(notScaledImage)
                            .scaled(w, h, Qt::KeepAspectRatioByExpanding);
    float scale = std::min((float)image.width() / notScaledImage.width(), (float)image.height() / notScaledImage.height());
    QPixmap target(image.size());

    QPainter p(&target);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawPixmap(0, 0, image);

    QList<double> values = _alcotester->getPersonsValues();
    QList<QRect> rects = _faceDetector->faceRects();
    int maxValueIndex = 0;
    double maxValue = 0;

    for (int i = 0; i < values.size(); i++) {
        if (values.at(i) > maxValue) {
            maxValue = values.at(i);
            maxValueIndex = i;
        }
    }

    for (int i = 0; i < values.size(); i++) {
        QRect faceRect = rects.at(i);
        QRect rect = QRect(faceRect.topLeft() * scale, faceRect.size() * scale);
        if (i != maxValueIndex) {
            drawLoser(p, rect, values.at(i));
        } else {
            drawChampion(p, rect, values.at(i));
        }
    }

    return target;
}

void PhotoPrintPageWidget::drawLoser(QPainter& p, const QRect& faceRect, double value)
{
    int faceRectRadius = std::max(faceRect.width(), faceRect.height()) / 2;
    int radius = 0.2 * faceRectRadius;
    QPoint center = faceRect.center() - QPoint(0, faceRectRadius);
    QRect rect(center.x() - radius, center.y() - radius
                           , radius * 2, radius * 2);
    QPen pen = QPen(QColor(255, 255, 255));
    pen.setWidth(faceRectRadius * 0.05);

    p.setPen(pen);
    p.setBrush(getAlcoLevelColor(value));
    p.drawEllipse(center, radius, radius);

    p.setFont(Utils::getFont("Proxima Nova Rg", radius, 0, QFont::Bold));
    QString valueText = QString::number(value);

    p.setPen(QColor(255, 255, 255));
    p.drawText(rect, Qt::AlignCenter, valueText);
}

void PhotoPrintPageWidget::drawChampion(QPainter& p, const QRect& faceRect, double value)
{
    int faceRectRadius = std::max(faceRect.width(), faceRect.height()) / 2;
    QPoint center = faceRect.center() - QPoint(0, faceRectRadius);
    int strokeWidth = faceRectRadius * 0.05;
    int radius = 0.2 * faceRectRadius;


    //rounded rect
    QPainterPath path;
    path.addRoundedRect(QRectF(center.x() - faceRect.width() / 2 + strokeWidth / 2
                               , center.y() - radius - strokeWidth / 2
                               , faceRect.width() + strokeWidth, 2 * radius + strokeWidth)
                               , radius + strokeWidth / 2, radius + strokeWidth / 2);
    p.fillPath(path, Qt::white);

    //winner text
    p.setFont(Utils::getFont("Proxima Nova Rg", radius, 0, QFont::Bold));
    p.setPen(Qt::black);
    // TODO: move text to config
    p.drawText(QRect(center.x() - faceRect.width() / 2 + 2 * radius
                     , center.y() - radius
                     , faceRect.width() - 2 * radius
                     , 2 * radius)
               , Qt::AlignCenter, "Победитель");


    //triangle

    float triangleSide = (faceRect.width() + strokeWidth) * 0.1;

    float x1 = center.x() - triangleSide / 2;
    float y1 = center.y() + radius + strokeWidth / 2;

    float x2   = center.x() + triangleSide / 2;
    float y2   = center.y() + radius + strokeWidth / 2;

    float x3 = center.x();
    float y3 = center.y() + radius + strokeWidth / 2 + (radius + strokeWidth / 2) * 0.4;

    QPolygon triangle;

    triangle << QPoint(x1, y1) << QPoint(x2, y2) << QPoint(x3, y3);

    p.setPen(Qt::NoPen);
    p.setBrush(Qt::white);
    p.drawPolygon(triangle);

    //alco circle
    center += QPoint(-faceRect.width() / 2 + radius, 0);

    QRect rect(center.x() - radius, center.y() - radius
                           , radius * 2, radius * 2);
    QPen pen = QPen(QColor(255, 255, 255));
    pen.setWidth(strokeWidth);
    p.setPen(pen);
    p.setBrush(getAlcoLevelColor(value));
    p.drawEllipse(center, radius, radius);

    p.setFont(Utils::getFont("Proxima Nova Rg", radius, 0, QFont::Bold));
    QString valueText = QString::number(value);

    p.setPen(QColor(255, 255, 255));
    p.drawText(rect, Qt::AlignCenter, valueText);
}

QColor PhotoPrintPageWidget::getAlcoLevelColor(double value) const
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
