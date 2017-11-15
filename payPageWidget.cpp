#include <QDateTime>

#include "payPageWidget.h"
#include "ui_payPageWidget.h"
#include "utils.h"
#include <functional>

PayPageWidget::PayPageWidget(QWidget *parent)
    : Page(parent)
    , _ui(new Ui::PayPageWidget)
    , _payStatePageNumber(initPayStatePageNumbers())
{
    _ui->setupUi(this);
}

PayPageWidget::~PayPageWidget()
{
    delete _ui;
}

void PayPageWidget::init(MainWindow *mainWindow)
{
    Page::init(mainWindow);

    _posDevice = mainWindow->getDeviceManager()->getPOSDevice();
}

QString PayPageWidget::getName() const
{
    return "pay";
}

QList<Transition*> PayPageWidget::getTransitions()
{
    QList<Transition*> transitions;

    // PAY -> SPLASH_SCREEN
    transitions.append(new Transition(PAY, SPLASH_SCREEN, [this](QEvent*) {
        // TODO: reset save image and data
        _mainWindow->setPage(SPLASH_SCREEN_PAGE);
    }));

    // PAY -> NOT_ENOUGH_MONEY
    transitions.append(new Transition(PAY, NOT_ENOUGH_MONEY, [this](QEvent*) {
        setNotEnoughMoneyInactionTimer();
        setSubPage(NOT_ENOUGH_MONEY);
    }));

    // PAY -> MORE_MONEY_THAN_NEED
    transitions.append(new Transition(PAY, MORE_MONEY_THAN_NEED, [this](QEvent*) {
        setInactionTimer("inactionMoreMoneyThanNeed");
        setSubPage(MORE_MONEY_THAN_NEED);
    }));

    // PAY -> PAYMENT_CONFIRMED
    transitions.append(new Transition(PAY, PAYMENT_CONFIRMED, [this](QEvent*) {
        setSubPage(PAYMENT_CONFIRMED);
        setInactionTimer("paymentConfirmed");
    }));

    // NOT_ENOUGH_MONEY -> SPLASH_SCREEN
    transitions.append(new Transition(NOT_ENOUGH_MONEY, SPLASH_SCREEN, [this](QEvent*) {
        _mainWindow->setPage(SPLASH_SCREEN_PAGE);
    }));

    // NOT_ENOUGH_MONEY -> PAY
    transitions.append(new Transition(NOT_ENOUGH_MONEY, PAY, [this](QEvent*) {
        setInactionTimer("inactionPayMoneyNonZero");
        setSubPage(PAY);
    }));

    // MORE_MONEY_THAN_NEED -> PAYMENT_CONFIRMED
    transitions.append(new Transition(MORE_MONEY_THAN_NEED, PAYMENT_CONFIRMED, [this](QEvent*) {
        setSubPage(PAYMENT_CONFIRMED);
    }));

    // PAYMENT_CONFIRMED -> ALCOTEST
    transitions.append(new Transition(PAYMENT_CONFIRMED, ALCOTEST, [this](QEvent*) {
        _mainWindow->setPage(ALCOTEST_PAGE);
    }));

    return transitions;
}

void PayPageWidget::onEntry()
{
    setSubPage(PAY);
    setInactionTimer("inactionPayMoneyZero");

    _price = 150; // TODO: use formula 50 * N instead of 150
    _ui->price->setText(QString(_priceText).replace("@PRICE", QString::number(_price)));

    _enteredMoneyAmount = 0;

    _posDevice->sendPrice(_price, [this](int statusCode) {
        // TODO: handle status code
        // while the test values are used
        if (statusCode == OK) {
            _posDevice->getPaymentResponce([=](int statusCode, int totalMoneyAmount) {
                if (_enteredMoneyAmount == 0 && totalMoneyAmount < _price) {
                    _timer.stop();
                    setInactionTimer("inactionPayMoneyNonZero");
                }

                _enteredMoneyAmount = totalMoneyAmount;

                if (_enteredMoneyAmount == _price) {
                    _mainWindow->goToState(PAYMENT_CONFIRMED);
                }

                if (_enteredMoneyAmount > _price) {
                    _mainWindow->goToState(MORE_MONEY_THAN_NEED);
                }
            });
        }
    });
}

void PayPageWidget::initInterface() {
    updateTexts(_ui->frame);
    retrieveTextTemplates();

    QString price = "150"; //TODO: get rid of it

    initPriceLabels(_ui->boldPrices->findChildren<QLabel*>()
                    , QString(_boldPricesText).replace("@PRICE", price));
    initPriceLabels(_ui->semiBoldPrices->findChildren<QLabel*>()
                    , QString(_semiBoldPricesText).replace("@PRICE", price));
    initPriceLabels(_ui->regularPrices->findChildren<QLabel*>()
                    , QString(_regularPricesText).replace("@PRICE", price));
    initPriceLabels(_ui->lightPrices->findChildren<QLabel*>()
                    , QString(_lightPricesText).replace("@PRICE", price));
}

void PayPageWidget::setConnections()
{

}

void PayPageWidget::initPriceLabels(QList<QLabel*> labels, const QString& richText)
{
    for (QLabel* label : labels) {
        label->setTextFormat(Qt::RichText);
        label->setText(richText);
    }
}

void PayPageWidget::setInactionTimer(const QString& durationName)
{
    int timeMs = _mainWindow->getConfigManager()->getTimeDuration(getName(), durationName) * 1000;

    _timer.setInterval(timeMs);

    QObject::disconnect(&_timer, &QTimer::timeout, 0, 0);
    QObject::connect(&_timer, &QTimer::timeout, [this]{
        _timer.stop();

        if (_enteredMoneyAmount == 0) {
            _mainWindow->goToState(SPLASH_SCREEN);
        } else if (_enteredMoneyAmount > 0 && _enteredMoneyAmount < _price) {
            _mainWindow->goToState(NOT_ENOUGH_MONEY);
        } else {
            _mainWindow->goToState(PAYMENT_CONFIRMED);
        }
    });

    _timer.start();
}

void PayPageWidget::setNotEnoughMoneyInactionTimer()
{
    int timeMs = _mainWindow->getConfigManager()->getTimeDuration(getName(), "inactionNotEnoughMoney") * 1000;
    _timerTimeLeft = timeMs;
    QString timerText = QDateTime::fromMSecsSinceEpoch(_timerTimeLeft).toString("mm:ss");
    _ui->timerText->setText(QString(_timerText).replace("@TIME", timerText));

    _timer.setInterval(1000); // each 1 second

    QObject::disconnect(&_timer, &QTimer::timeout, 0, 0);
    QObject::connect(&_timer, &QTimer::timeout, [this]{
        _timerTimeLeft -= 1000;
        QString timerText = QDateTime::fromMSecsSinceEpoch(_timerTimeLeft).toString("mm:ss");
        _ui->timerText->setText(QString(_timerText).replace("@TIME", timerText));

        if (_timerTimeLeft <= 0) {
            _timer.stop();
            _mainWindow->goToState(SPLASH_SCREEN);
        }
    });

    _timer.start();
}

void PayPageWidget::retrieveTextTemplates()
{
    QList<Text> texts = _mainWindow->getConfigManager()->getLanguageIndependentText("pay");

    for (const Text t : texts) {
        if (t.getName() == "boldPrices") {
            _boldPricesText = t.getText();
        }

        if (t.getName() == "semiBoldPrices") {
            _semiBoldPricesText = t.getText();
        }

        if (t.getName() == "regularPrices") {
            _regularPricesText = t.getText();
        }

        if (t.getName() == "lightPrices") {
            _lightPricesText = t.getText();
        }

        if (t.getName() == "price") {
            _priceText = t.getText();
        }
    }

    retrieveLanguageDependentTemplates();
}

void PayPageWidget::retrieveLanguageDependentTemplates()
{
    QList<Text> texts = _mainWindow->getConfigManager()->getTexts("pay");

    for (const Text t : texts) {
        if (t.getName() == "timerText") {
            _timerText = t.getText();
        }
    }
}

QMap<StateName, int> PayPageWidget::initPayStatePageNumbers() const
{
    QMap<StateName, int> map;

    map.insert(PAY, 0);
    map.insert(NOT_ENOUGH_MONEY, 1);
    map.insert(MORE_MONEY_THAN_NEED, 2);
    map.insert(PAYMENT_CONFIRMED, 3);

    return map;
}

void PayPageWidget::setSubPage(StateName stateName)
{
    int payStateIndex = _payStatePageNumber[stateName];

    _ui->payState->setCurrentIndex(payStateIndex);
}
