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
    Page::init(mainWindow);
    initInterface();
}

QString AlcoTestPageWidget::getName() const
{
    return "test";
}

void AlcoTestPageWidget::initInterface()
{
    updateTexts(_ui->frame);
}
