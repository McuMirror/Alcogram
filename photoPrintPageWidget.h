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
    void initInterface() override;

private:
    void setTimer(const QString& durationName);

    Ui::PhotoPrintPageWidget* _ui;
    PrinterInterface* _printer;
    CameraInterface* _camera;
    AlcotesterInterface* _alcotester;
    FaceDetectionInterface* _faceDetector;
};
