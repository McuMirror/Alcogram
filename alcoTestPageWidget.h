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
    // callback on Machinary signals for alcotester device
    void onReceivedAlcotesterData(QSharedPointer<Status> status, double value);
    void onFailedToReceiveDataFromAcotester(QSharedPointer<Status> status);
    void onAlcotesterError(QSharedPointer<Status> status);
    void onWarmingUpAlcotester(QSharedPointer<Status> status);
    void onRestartAlcotester(QSharedPointer<Status> status);

    // set timer
    // @param durationName - name of duration
    void setTimer(const QString& durationName);

    // run alcotest
    // @param i - number of person
    void test(int i);

    // select the face of the test person
    void circleCurrentPerson();

    // draw alcovalues of previously tested persons
    void drawPreviousPersonValues(QPainter& p, float scale);

    // draw alcolevel name
    void drawSuccesText(QPainter &p, int textSize, QPoint pos);

    // get alcolevel name for currently test person
    QString getAlcoLevelName() const;

    // get color for alcovalue
    QColor getAlcoLevelColor(double value) const;

    Ui::AlcoTestPageWidget* _ui;
    FaceDetectionInterface* _faceDetector;

    QPixmap _repeatIcon;

    AlcotestCircleState _circleState = TEST;
    double _lastPersonValue;
    int _currentPerson; // current person number
    int _alcotesterWarmingUpAttemptNumber; // number of attempts to warm up alcotester in succession
    int _alcotesterFailureNumber; // number of attempts to read data from alcotester in succession
    bool _alcotesterInWork;

    QMap<QString, QPair<float, float>> _alcoLevelIntervals; // interval of alcovalues for each alcolevel
};
