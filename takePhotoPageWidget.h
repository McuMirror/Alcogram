#pragma once

#include <QWidget>
#include <QTimer>
#include <QImage>
#include <QThread>
#include <QSharedPointer>

#include "page.h"

//#include "videoFaceDetector.h"
#include "interfaces/faceDetectionInterface.h"
#include "deviceInterfaces/cameraInterface.h"
#include "cameraImageHandler.h"

namespace Ui {
    class TakePhotoPageWidget;
}

class TakePhotoPageWidget : public Page
{
    Q_OBJECT

public:
    explicit TakePhotoPageWidget(QWidget *parent = 0);
    ~TakePhotoPageWidget();

    void init(MainWindowInterface* mainWindow) override;
    QString getName() const override;
    QList<Transition*> getTransitions() override;

signals:
    void startImageHandle();

protected:
    void onEntry() override;
    void onExit() override;
    void initInterface() override;
    void setConnections() override;

private:
    // callbacks for Machinery signals for camera device
    void onNextFrameReceived(QSharedPointer<QImage> image, QSharedPointer<Status> status);
    void onReceivedCapturedImage(QSharedPointer<QImage> image, QSharedPointer<Status> status);
    void onCameraRestart(QSharedPointer<Status> status);
    void onCameraError(QSharedPointer<Status> status);

    // calls after received signal processed from CameraImageHandler
    void updateCameraOutput(QPixmap processedImage);

    // setting photo timer with duration named "timer"
    void setPhotoTimer();

    // setting inaction timer with duration named durationName
    void setInactionTimer(const QString& durationName);

    // returns bottomPanel page numbers related to specific state
    QMap<StateName, int> initBottomPanelPageNumbers() const;

    // returns mainPanel page numbers related to specific state
    QMap<StateName, int> initMainPanelPageNumbers() const;

    // setting page for bottomPanel and mainPanel by stateName
    void setSubPage(StateName stateName);

    void setErrorSubPage(const QString& textName = "", const QString& durationName = "");

    Ui::TakePhotoPageWidget* _ui;
    int _timerTimeLeft; // remaining time for photo timer

    QMap<StateName, int> _bottomPanelPageNumbers; // state -> bottomPanel page number
    QMap<StateName, int> _mainPanelPageNumbers; // state -> mainPanel page number

    FaceDetectionInterface* _faceDetector;
    QThread _imageProcessingThread;
    CameraImageHandler _cameraImageHandler;

    bool _isImageHandling = false;
    bool _isImageCapturing = false;

    static const int ERROR_SUBPAGE_MAIN = 2;
    static const int ERROR_SUBPAGE_BOTTOM = 3;
};
