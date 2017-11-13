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


void TakePhotoPageWidget::init(MainWindow* mainWindow)
{
    Page::init(mainWindow);

    initInterface();

    //_ui->mainPanel->setCurrentIndex(1);
}

QString TakePhotoPageWidget::getName() const
{
    return "take";
}

QList<TransitionPack> TakePhotoPageWidget::getTransitions()
{
    return Page::getTransitions();
}

void TakePhotoPageWidget::initInterface()
{
    updateTexts(_ui->frame);

    //timer font
    _ui->timer->setFont(Utils::getFont("Proxima Nova Rg", 47, 2, QFont::Bold));

    //takenPhoto shadow
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(128);
    effect->setColor(QColor(0, 0, 0, 127));
    effect->setOffset(0, 42);

    _ui->takenPhoto->setGraphicsEffect(effect);
}
