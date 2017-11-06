#include "alcoTestPageWidget.h"
#include "ui_alcoTestPageWidget.h"
#include "utils.h"

AlcoTestPageWidget::AlcoTestPageWidget(QWidget *parent) :
    Page(parent),
    _ui(new Ui::AlcoTestPageWidget)
{
    _ui->setupUi(this);
}

AlcoTestPageWidget::~AlcoTestPageWidget()
{
    delete _ui;
}

void AlcoTestPageWidget::init(MainWindow* mainWindow)
{
    initInterface();
}

void AlcoTestPageWidget::initInterface()
{
    //steps text font
    QFont font = Utils::getFont("Proxima Nova Rg", 20, 2, QFont::Bold);

    _ui->stepOne->setFont(font);
    _ui->stepTwo->setFont(font);
    _ui->stepThree->setFont(font);
    _ui->stepFour->setFont(font);
}
