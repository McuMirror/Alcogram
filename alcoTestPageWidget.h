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
    void onExit() override;
    void initInterface() override;
    void setConnections() override;

private:
    void onReceivedAlcotesterData(QSharedPointer<Status> status, double value);
    void onFailedToReceiveDataFromAcotester(QSharedPointer<Status> status);
    void onError(QSharedPointer<Status> status);
    void onWarmingUpAlcotester(QSharedPointer<Status> status);
    void onRestartAlcotester(QSharedPointer<Status> status);

    void setTimer(const QString& durationName);
    void test(int i);
    void circleCurrentPerson();
    void drawPreviousPersonValues(QPainter& p, float scale);
    void drawSuccesText(QPainter &p, int textSize, QPoint pos);
    QString getAlcoLevelName() const;
    QColor getAlcoLevelColor(double value) const;

    Ui::AlcoTestPageWidget* _ui;
    FaceDetectionInterface* _faceDetector;

    QPixmap _repeatIcon;

    AlcotestCircleState _circleState = TEST;
    double _lastPersonValue;
    int _currentPerson;
    int _alcotesterWarmingUpAttemptNumber;
    int _alcotesterFailureNumber;
    bool _alcotesterInWork;

    QMap<QString, QPair<float, float>> _alcoLevelIntervals;
};
