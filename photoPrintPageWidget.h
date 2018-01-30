#pragma once

#include <QWidget>
#include <QTimer>

#include "page.h"
#include "deviceInterfaces/printerinterface.h"
#include "deviceInterfaces/alcotesterinterface.h"
#include "deviceInterfaces/cameraInterface.h"
#include "interfaces/faceDetectionInterface.h"


namespace Ui {
    class PhotoPrintPageWidget;
}

class PhotoPrintPageWidget : public Page
{
    Q_OBJECT

public:
    explicit PhotoPrintPageWidget(QWidget *parent = 0);
    ~PhotoPrintPageWidget();

    void init(MainWindow* mainWindow) override;
    QString getName() const override;
    QList<Transition*> getTransitions() override;

protected:
    void onEntry() override;
    void onExit() override;
    void initInterface() override;
    void setConnections() override;

private:
    // callback for Machinery signals for printer device
    void onImagePrinted(QSharedPointer<Status> status);
    void onPrinterCheckStatus(QSharedPointer<Status> status);
    void onPrinterWarmUp(QSharedPointer<Status> status);
    void onPrinterCoolDown(QSharedPointer<Status> status);
    void onError(QSharedPointer<Status> status);
    void onPrinterRestart(QSharedPointer<Status> status);

    void setTimer(const QString& durationName);
    QPixmap generateFinalPhoto(int w, int h);
    void drawLoser(QPainter& p, const QRect& faceRect, double value);
    void drawChampion(QPainter& p, const QRect& faceRect, double value);
    QColor getAlcoLevelColor(double value) const;

    Ui::PhotoPrintPageWidget* _ui;
    FaceDetectionInterface* _faceDetector;
    QMap<QString, QPair<float, float>> _alcoLevelIntervals;
    int _printedPhotos; // number of printed photos
    int _printerWarmingUpAttemptNumber; // number of attempts to warm up printer in succession
    int _printerRestartFailureNumber; // number of attempts to restart printer in succession
    int _printerPrintFailureNumber; // number of attempts to print image in succession
    bool _wasPrintFailure; // print failure
};
