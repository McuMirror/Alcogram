#include "splashScreenEternalSleep.h"
#include "ui_splashScreenEternalSleep.h"

SplashScreenEternalSleep::SplashScreenEternalSleep(QWidget *parent) :
    Page(parent),
    ui(new Ui::SplashScreenEternalSleep)
{
    ui->setupUi(this);
}

SplashScreenEternalSleep::~SplashScreenEternalSleep()
{
    delete ui;
}

void SplashScreenEternalSleep::init(MainWindow *mainWindow)
{
    Page::init(mainWindow);
}

QString SplashScreenEternalSleep::getName() const
{
    return "splashScreenEternalSleep";
}
