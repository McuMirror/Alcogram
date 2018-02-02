#include <QFontDatabase>
#include <QDir>
#include <QDebug>
#include <QMouseEvent>

#include "mainWindow.h"
#include "ui_mainWindow.h"
#include "faceDetector.h"
#include "logger.h"

const PageName MainWindow::_statePageNames[] = {
    SPLASH_SCREEN_PAGE // SPLASH_SCREEN state
    , START_PAGE // START state
    , SPLASH_SCREEN_ETERNAL_SLEEP_PAGE // SPLASH_SCREEN_ETERNAL_SLEEP
    , SPLASH_SCREEN_NONCRITICAL_ERROR_PAGE// SPLASH_SCREEN_NONCRITICAL_ERROR
    , TAKE_PHOTO_PAGE // PREPARING_FOR_PHOTO
    , TAKE_PHOTO_PAGE //PHOTO_TIMER
    , TAKE_PHOTO_PAGE // PHOTO_CONFIRMATION
    , PAY_PAGE // PAY
    , PAY_PAGE // NOT_ENOUGH_MONEY
    , PAY_PAGE // MORE_MONEY_THAN_NEED
    , PAY_PAGE // PAYMENT_CONFIRMED
    , ALCOTEST_PAGE // ALCOTEST
    , ALCOTEST_PAGE // DRUNKENESS_NOT_RECOGNIZED
    , ALCOTEST_PAGE // ALCOTEST_INACTION
    , PHOTO_PRINT_PAGE // FINAL_PHOTO
    , PHOTO_PRINT_PAGE // PHOTO_PRINT
    };

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    , _ui(new Ui::MainWindow)
    , _configManager(new ConfigManager(this))
    , _stateMachine(new StateMachine(this))
    , _faceDetector(new FaceDetector(this))
    , _machinery(new Machinery(this))
    , _devicesChecker(this, this)
{
    _ui->setupUi(this);

    Logger::instance()->setMainWindow(this);

    qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::ALCOGRAM_START);
    qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::INITIALIZATION_START);

    _configManager->load(":/config.conf");

    loadFonts();

    //init pages
    for (int i = 0; i < _ui->pages->count(); i++) {
        Page* page = static_cast<Page*>(_ui->pages->widget(i));

        page->init(this);
        _stateMachine->addTransitions(page->getTransitions());
    }

    qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::INITIALIZATION_FINISH);

    QObject::connect(_stateMachine, &StateMachine::criticalError, this, &MainWindow::criticalError);
    QObject::connect(_stateMachine, &StateMachine::fromCriticalError, this, &MainWindow::onFromCriticalError);

    _stateMachine->run();
    setPage(SPLASH_SCREEN_PAGE);
}

void MainWindow::setPage(PageName pageName)
{
    static_cast<Page*>(_ui->pages->currentWidget())->exit();
    _ui->pages->setCurrentIndex(pageName);
    static_cast<Page*>(_ui->pages->currentWidget())->enter();
}

ConfigManager* MainWindow::getConfigManager() const
{
    return _configManager;
}

Machinery* MainWindow::getMachinery() const
{
    return _machinery;
}

FaceDetectionInterface* MainWindow::getFaceDetector() const
{
    return _faceDetector;
}

void MainWindow::goToState(StateName targetState)
{
    _stateMachine->postEvent(new Event(targetState, getCurrentStateName()));
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    qDebug().noquote() << Logger::instance()->buildTapLog(event->x(), event->y());
}


SessionData& MainWindow::getSessionData()
{
    return _sessionData;
}

DevicesChecker& MainWindow::getDevicesChecker()
{
    return _devicesChecker;
}

void MainWindow::switchLanguage()
{
    _configManager->switchLanguage();

    for (int i = 0; i < _ui->pages->count(); i++) {
        Page* page = static_cast<Page*>(_ui->pages->widget(i));

        page->switchLanguage();
    }
}

void MainWindow::onFromCriticalError(StateName toState)
{
    _ui->pages->setCurrentIndex(_statePageNames[static_cast<int>(toState)]);
}

void MainWindow::loadFonts()
{
    QDir directory(":/fonts/");
    QStringList fontList = directory.entryList(QStringList("*.ttf"));
    QString ff;

    for (QString font : fontList) {
        int id = QFontDatabase::addApplicationFont(":/fonts/" + font);
        _fonts.append(id);
        ff = QFontDatabase::applicationFontFamilies(id).at(0);
    }
}

void MainWindow::removeFonts()
{
    for (int id : _fonts) {
        QFontDatabase::removeApplicationFont(id);
    }
}

MainWindow::~MainWindow()
{
    delete _ui;

    removeFonts();

    qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::ALCOGRAM_FINISH);

    Logger::instance()->resetInstance();
}
