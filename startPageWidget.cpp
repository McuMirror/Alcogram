#include <QGraphicsDropShadowEffect>
#include <QFontDatabase>

#include "startPageWidget.h"
#include "ui_startPageWidget.h"

StartPageWidget::StartPageWidget(QWidget *parent) :
    Page(parent),
    _ui(new Ui::StartPageWidget)
{
    _ui->setupUi(this);
}

void StartPageWidget::init(MainWindow* mainWindow) {
    _mainWindow = mainWindow;

    initInterface(_mainWindow->getFontFamily());

    QObject::connect(_ui->startButton, &QPushButton::released, this, &StartPageWidget::onStartButtonRelease);
}

void StartPageWidget::onStartButtonRelease() {
    _mainWindow->nextPage();
}

void StartPageWidget::initInterface(const QString& family) {
    //startButton font and text color
    QFont font(family);
    font.setPixelSize(28);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 4);
    font.setBold(true);

    _ui->startButton->setFont(font);

    //startButton shadow
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);

    effect->setBlurRadius(142);
    effect->setColor(QColor(0, 0, 0, 117));
    effect->setOffset(0, 57);
    _ui->startButton->setGraphicsEffect(effect);

    //drunk womens photo shadow
    effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(128);
    effect->setColor(QColor(0, 0, 0, 127));
    effect->setOffset(0, 42);
    _ui->drunkWomensPhoto->setGraphicsEffect(effect);

    //text 01, 02, 03 font
    font = QFont(family);
    font.setPixelSize(200);
    font.setBold(true);

    _ui->label01->setFont(font);
    _ui->label02->setFont(font);
    _ui->label03->setFont(font);

    //step descriptions font
    font = QFont(family);
    font.setPixelSize(29);
    font.setBold(true);

    _ui->firstStepDescription->setFont(font);
    _ui->sndStepDescription->setFont(font);
    _ui->thirdStepDescription->setFont(font);

    //switch language button font
    _ui->switchLanguageButton->setFont(font);

    //alcogram text font
    font = QFont(family);
    font.setPixelSize(28);
    _ui->alcogramText->setFont(font);
}

StartPageWidget::~StartPageWidget()
{
    delete _ui;
}
