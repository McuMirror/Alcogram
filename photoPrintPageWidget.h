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
    void setConnections() override;

private:
    void onImagePrinted(QSharedPointer<Status> status);

    void setTimer(const QString& durationName);
    QPixmap generateFinalPhoto(int w, int h);
    void drawLoser(QPainter& p, const QRect& faceRect, double value);
    void drawChampion(QPainter& p, const QRect& faceRect, double value);
    QColor getAlcoLevelColor(double value) const;

    Ui::PhotoPrintPageWidget* _ui;
    FaceDetectionInterface* _faceDetector;
    QMap<QString, QPair<float, float>> _alcoLevelIntervals;
    int _printedPhotos;
};
