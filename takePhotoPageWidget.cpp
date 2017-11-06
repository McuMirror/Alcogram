#include <QGraphicsDropShadowEffect>

#include "takePhotoPageWidget.h"
#include "ui_takePhotoPageWidget.h"
#include "utils.h"

TakePhotoPageWidget::TakePhotoPageWidget(QWidget *parent) :
    Page(parent),
    _ui(new Ui::TakePhotoPageWidget)
{
    _ui->setupUi(this);
}

TakePhotoPageWidget::~TakePhotoPageWidget()
{
    delete _ui;
}


void TakePhotoPageWidget::init(MainWindow* mainWindow) {
    initInterface();

    //_ui->mainPanel->setCurrentIndex(1);
}

void TakePhotoPageWidget::initInterface() {
    //steps text font
    QFont font = Utils::getFont("Proxima Nova Rg", 20, 2, QFont::Bold);

    _ui->stepOne->setFont(font);
    _ui->stepTwo->setFont(font);
    _ui->stepThree->setFont(font);
    _ui->stepFour->setFont(font);

    //timer font
    _ui->timer->setFont(Utils::getFont("Proxima Nova Rg", 47, 2, QFont::Bold));

    //takenPhoto shadow
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(128);
    effect->setColor(QColor(0, 0, 0, 127));
    effect->setOffset(0, 42);

    _ui->takenPhoto->setGraphicsEffect(effect);

    //retake, continue, take text font
    font = Utils::getFont("Proxima Nova Rg", 27, 4, QFont::Bold);

    _ui->retakePhototText->setFont(font);
    _ui->continueText->setFont(font);
    _ui->takePhotoText->setFont(font);
}
