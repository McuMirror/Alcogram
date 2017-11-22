#pragma once

#include <QWidget>
#include <QTimer>
#include <QPixmap>

#include "page.h"
#include "deviceInterfaces/alcotesterinterface.h"
#include "deviceInterfaces/cameraInterface.h"
#include "interfaces/faceDetectionInterface.h"

namespace Ui {
    class AlcoTestPageWidget;
}

enum AlcotestCircleState {
    TEST
    , FAIL
    , SUCCESS
};

class AlcoTestPageWidget : public Page
{
    Q_OBJECT

public:
    explicit AlcoTestPageWidget(QWidget *parent = 0);
    ~AlcoTestPageWidget();

    void init(MainWindow* mainWindow) override;
    QString getName() const override;
    QList<Transition*> getTransitions() override;
    void onEntry() override;

protected:
    void initInterface() override;

private:
    void setTimer(const QString& durationName);
    void test(int i);
    void circleCurrentPerson();

    Ui::AlcoTestPageWidget* _ui;
    QTimer _timer;
    AlcotesterInterface* _alcotester;
    FaceDetectionInterface* _faceDetector;
    CameraInterface* _camera;

    QPixmap _repeatIcon;

    AlcotestCircleState _circleState = TEST;
    double _lastPersonValue;
    int _currentPerson;
};
