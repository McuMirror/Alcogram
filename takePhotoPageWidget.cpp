#include <QGraphicsDropShadowEffect>

#include "takePhotoPageWidget.h"
#include "ui_takePhotoPageWidget.h"

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
    initInterface(mainWindow->getFontFamily());

    _ui->mainPanel->setCurrentIndex(1);
}

void TakePhotoPageWidget::initInterface(const QString& family) {
    //takePhotoText font
    QFont font(family);
    font.setPixelSize(27);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 4);
    font.setBold(true);

    _ui->takePhotoText->setFont(font);

    //steps text font
    font = QFont(family);
    font.setPixelSize(20);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 2);
    font.setBold(true);

    _ui->stepOne->setFont(font);
    _ui->stepTwo->setFont(font);
    _ui->stepThree->setFont(font);
    _ui->stepFour->setFont(font);

    //timer font
    font = QFont(family);
    font.setPixelSize(47);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 2);
    font.setBold(true);

    _ui->timer->setFont(font);

    //takenPhoto shadow
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(128);
    effect->setColor(QColor(0, 0, 0, 127));
    effect->setOffset(0, 42);

    _ui->takenPhoto->setGraphicsEffect(effect);

    //retake text font
    font = QFont(family);
    font.setPixelSize(27);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 4);
    font.setBold(true);

    _ui->retakePhototText->setFont(font);

    //continue text font
    font = QFont(family);
    font.setPixelSize(27);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 4);
    font.setBold(true);

    _ui->continueText->setFont(font);
}
