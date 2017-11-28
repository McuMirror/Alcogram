#include <functional>
#include <QDebug>

#include "splashScreen.h"
#include "ui_splashScreen.h"
#include "stateMachine.h"

#include "logger.h"

using namespace std::placeholders;

SplashScreen::SplashScreen(QWidget *parent) :
    Page(parent),
    _ui(new Ui::SplashScreen)
{
    _ui->setupUi(this);
}

SplashScreen::~SplashScreen()
{
    delete _ui;
}

void SplashScreen::init(MainWindow *mainWindow)
{
    Page::init(mainWindow);

    QObject::connect(_ui->frameButton, &QPushButton::released, [=] {
        _mainWindow->goToState(START);
    });
}

QString SplashScreen::getName() const
{
    return "splashScreen";
}

QList<Transition*> SplashScreen::getTransitions()
{
    Transition* transition = new Transition(SPLASH_SCREEN, START, [=](QEvent*){
        _mainWindow->setPage(START_PAGE);
    });

    return QList<Transition*>({transition});
}
