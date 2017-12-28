#pragma once

#include <QWidget>
#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include <QMap>

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

protected:
    void onEntry() override;
    void initInterface() override;

private:
    void setTimer(const QString& durationName);
    void test(int i);
    void circleCurrentPerson();
    void drawPreviousPersonValues(QPainter& p, float scale);
    void drawSuccesText(QPainter &p, int textSize, QPoint pos);
    QString getAlcoLevelName() const;
    QColor getAlcoLevelColor(double value) const;

    Ui::AlcoTestPageWidget* _ui;
    AlcotesterInterface* _alcotester;
    FaceDetectionInterface* _faceDetector;
    CameraInterface* _camera;

    QPixmap _repeatIcon;

    AlcotestCircleState _circleState = TEST;
    double _lastPersonValue;
    int _currentPerson;

    QMap<QString, QPair<float, float>> _alcoLevelIntervals;
};
