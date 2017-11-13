#include <functional>

#include "splashScreen.h"
#include "ui_splashScreen.h"
#include "stateMachine.h"

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

    QObject::connect(_ui->frameButton, &QPushButton::released, [=]{
        _mainWindow->postEvent(new Event(TO_START_0_2, SPLASH_SCREEN));
    });
}

QString SplashScreen::getName() const
{
    return "splashScreen";
}

QList<TransitionPack> SplashScreen::getTransitions()
{
    Transition* transition = new Transition(TO_START_0_2, std::bind(&SplashScreen::toStartPage, this));
    return QList<TransitionPack>({TransitionPack(transition, SPLASH_SCREEN, START)});
}

void SplashScreen::toStartPage()
{
    _mainWindow->setPage(START_PAGE);
}
