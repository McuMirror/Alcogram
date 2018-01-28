#include <QFontDatabase>
#include <QDir>
#include <QDebug>
#include <QMouseEvent>

#include "mainWindow.h"
#include "ui_mainWindow.h"
#include "faceDetector.h"
#include "logger.h"

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

    _stateMachine->run();
    setPage(SPLASH_SCREEN_PAGE);
}

void MainWindow::setPage(PageName pageName)
{
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
