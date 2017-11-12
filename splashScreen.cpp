#include "splashScreen.h"
#include "ui_splashScreen.h"

SplashScreen::SplashScreen(QWidget *parent) :
    Page(parent),
    ui(new Ui::SplashScreen)
{
    ui->setupUi(this);
}

SplashScreen::~SplashScreen()
{
    delete ui;
}

void SplashScreen::init(MainWindow *mainWindow)
{

}

QString SplashScreen::getName() const
{
    return "splashScreen";
}
