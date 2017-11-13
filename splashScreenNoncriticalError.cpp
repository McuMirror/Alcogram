#include "splashScreenNoncriticalError.h"
#include "ui_splashScreenNoncriticalError.h"

SplashScreenNoncriticalError::SplashScreenNoncriticalError(QWidget *parent) :
    Page(parent),
    ui(new Ui::SplashScreenNoncriticalError)
{
    ui->setupUi(this);
}

SplashScreenNoncriticalError::~SplashScreenNoncriticalError()
{
    delete ui;
}

void SplashScreenNoncriticalError::init(MainWindow *mainWindow)
{
    Page::init(mainWindow);
}

QString SplashScreenNoncriticalError::getName() const
{
    return "splashScreenNoncriticalError";
}
