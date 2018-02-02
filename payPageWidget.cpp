#include <QDateTime>
#include <QDebug>
#include <functional>

#include "payPageWidget.h"
#include "ui_payPageWidget.h"
#include "utils.h"
#include "logger.h"


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

void PayPageWidget::init(MainWindowInterface* mainWindow)
{
    Page::init(mainWindow);
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
        stopTimer();
        _mainWindow->getMachinery()->deactivateBillAcceptor();
        _mainWindow->getMachinery()->deactivatePOS();
        _mainWindow->setPage(SPLASH_SCREEN_PAGE);
    }));

    // PAY -> NOT_ENOUGH_MONEY
    transitions.append(new Transition(PAY, NOT_ENOUGH_MONEY, [this](QEvent*) {
        _ui->continueButton->show();
        setNotEnoughMoneyInactionTimer();
        setSubPage(NOT_ENOUGH_MONEY);
    }));

    // PAY -> MORE_MONEY_THAN_NEED
    transitions.append(new Transition(PAY, MORE_MONEY_THAN_NEED, [this](QEvent*) {
        _ui->continueButton->show();
       _mainWindow->getMachinery()->deactivateBillAcceptor();
       _mainWindow->getMachinery()->deactivatePOS();
        setSubPage(MORE_MONEY_THAN_NEED);
        setInactionTimer("inactionMoreMoneyThanNeed");
    }));

    // PAY -> PAYMENT_CONFIRMED
    transitions.append(new Transition(PAY, PAYMENT_CONFIRMED, [this](QEvent*) {
        _ui->cancelButton->hide();
       _mainWindow->getMachinery()->deactivateBillAcceptor();
       _mainWindow->getMachinery()->deactivatePOS();
        setSubPage(PAYMENT_CONFIRMED);
        setInactionTimer("paymentConfirmed");
    }));

    // NOT_ENOUGH_MONEY -> SPLASH_SCREEN
    transitions.append(new Transition(NOT_ENOUGH_MONEY, SPLASH_SCREEN, [this](QEvent*) {
        _mainWindow->getMachinery()->deactivateBillAcceptor();
        _mainWindow->getMachinery()->deactivatePOS();
        _mainWindow->setPage(SPLASH_SCREEN_PAGE);
    }));

    // NOT_ENOUGH_MONEY -> PAY
    transitions.append(new Transition(NOT_ENOUGH_MONEY, PAY, [this](QEvent*) {
        _ui->continueButton->hide();
        setInactionTimer("inactionPayMoneyNonZero");
        setSubPage(PAY);
    }));

    // MORE_MONEY_THAN_NEED -> PAYMENT_CONFIRMED
    transitions.append(new Transition(MORE_MONEY_THAN_NEED, PAYMENT_CONFIRMED, [this](QEvent*) {
        _ui->continueButton->hide();
        _ui->cancelButton->hide();
        setSubPage(PAYMENT_CONFIRMED);
        setInactionTimer("paymentConfirmed");
    }));

    // PAYMENT_CONFIRMED -> ALCOTEST
    transitions.append(new Transition(PAYMENT_CONFIRMED, ALCOTEST, [this](QEvent*) {
        stopTimer();
        _mainWindow->setPage(ALCOTEST_PAGE);
    }));

    return transitions;
}

void PayPageWidget::onEntry()
{
    _ui->continueButton->hide();
    _ui->cancelButton->show();

    setSubPage(PAY);

    // set inaction timer on zero money case
    setInactionTimer("inactionPayMoneyZero");

    // compute price
    _price = _mainWindow->getFaceDetector()->facesCount() * 50;

    // set backgroung
    _ui->price->setText(QString(_priceText).replace("@PRICE", QString::number(_price)));
    setPriceToPriceLabels(_price);

    _enteredMoneyAmount = 0;
    _billAcceptorActive = false;
    _posActive = false;

    QObject::connect(_mainWindow->getMachinery(), &Machinery::error, this, &PayPageWidget::onError);

    // activate POS and bill acceptor devices
    _mainWindow->getMachinery()->activatePOS();
    _mainWindow->getMachinery()->activateBillAcceptor();
}

void PayPageWidget::onExit()
{
    QObject::disconnect(_mainWindow->getMachinery(), &Machinery::error, this, &PayPageWidget::onError);
}

void PayPageWidget::initInterface() {
    updateTexts(_ui->frame);
    retrieveTextTemplates();
}

void PayPageWidget::setConnections()
{
    QObject::connect(_ui->continueButton, &QPushButton::released, [this] {
        qDebug().noquote() << Logger::instance()->buildUserActionLog(Logger::BUTTON_RELEASE, Logger::BUTTON
            , _ui->continueButton->objectName());


        _mainWindow->goToState(_mainWindow->getCurrentStateName() == NOT_ENOUGH_MONEY
                               ? PAY : PAYMENT_CONFIRMED);
    });

    QObject::connect(_ui->cancelButton, &QPushButton::released, [this] {
        StateName targetState;

        switch (_mainWindow->getCurrentStateName()) {
        case NOT_ENOUGH_MONEY:
            targetState = PAY;
            break;
        case MORE_MONEY_THAN_NEED:
            targetState = PAYMENT_CONFIRMED;
            break;
        case PAY:
            targetState = SPLASH_SCREEN;
            break;
        default:
            // TODO: wrong state
            return;
        }

        qDebug().noquote() << Logger::instance()->buildUserActionLog(Logger::BUTTON_RELEASE, Logger::BUTTON
            , _ui->cancelButton->objectName(), QList<double>()
            , QStringList({Utils::getStateNameNumber(targetState)}));

        _mainWindow->goToState(targetState);
    });

    Machinery* machinery = _mainWindow->getMachinery();

    QObject::connect(machinery, &Machinery::transactionSucceded
                     , this, &PayPageWidget::onTransactionSucceded);

    QObject::connect(machinery, &Machinery::transactionFailed
                     , this, &PayPageWidget::onTransactionFailed);

    QObject::connect(machinery, &Machinery::posActivated
                     , this, &PayPageWidget::onActivatePOS);

    QObject::connect(machinery, &Machinery::moneyReceived
                     , this, &PayPageWidget::onMoneyReceived);

    QObject::connect(machinery, &Machinery::moneyRejected
                     , this, &PayPageWidget::onMoneyRejected);

    QObject::connect(machinery, &Machinery::billAcceptorActivated
                     , this, &PayPageWidget::onActivateBillAcceptor);
}

void PayPageWidget::onTransactionSucceded(double money, QSharedPointer<Status> status)
{
    // successfull POS transaction
    _enteredMoneyAmount = money;
    _mainWindow->getMachinery()->deactivatePOS();
    _mainWindow->getMachinery()->deactivateBillAcceptor();
    _mainWindow->goToState(PAYMENT_CONFIRMED);
}

void PayPageWidget::onTransactionFailed(QSharedPointer<Status> status)
{
    // unsuccessfull POS transation
    // TODO: show msg
    _mainWindow->getMachinery()->takeMoney(_price);
}

void PayPageWidget::onMoneyReceived(double money, QSharedPointer<Status> status)
{
    // banknote adopted
    _enteredMoneyAmount += money;

    if (_enteredMoneyAmount > 0 && _enteredMoneyAmount < _price) {
        setInactionTimer("inactionNotEnoughMoney");
    }

    if (_enteredMoneyAmount == _price) {
        _mainWindow->goToState(PAYMENT_CONFIRMED);

        return;
    }

    if (_enteredMoneyAmount > _price) {
        _mainWindow->goToState(MORE_MONEY_THAN_NEED);
    }

    _ui->price->setText(QString(_ofPriceText).replace("@PRICE", QString::number(_price))
                        .replace("@AMOUNT", QString::number(_enteredMoneyAmount)));
}

void PayPageWidget::onMoneyRejected(QSharedPointer<Status> status)
{
    // failing banknote
}

void PayPageWidget::onError(QSharedPointer<Status> status)
{
    // TODO: add actual errors
    switch(status->getRequestName()) {
    case TAKE_MONEY:
    case ACTIVATE_POS:
        // TODO: show msg
        _posActive = false;
        _mainWindow->getMachinery()->restart(POS);

        if (!_billAcceptorActive) {
            _mainWindow->goToState(CRITICAL_ERROR);
        }
        break;
    case MONEY_REQUEST:
    case ACTIVATE_BILL_ACCEPTOR:
        // TODO: show msg
        _billAcceptorActive = false;
        _mainWindow->getMachinery()->restart(BILL_ACCEPTOR);

        if (!_posActive) {
             _mainWindow->goToState(CRITICAL_ERROR);
        }
        break;
    case DEACTIVATE_POS:
        break;
    case RESTART_DEVICE:
        break;
    }

}

void PayPageWidget::onActivatePOS(QSharedPointer<Status> status)
{
    _posActive = true;
    _mainWindow->getMachinery()->takeMoney(_price);
}

void PayPageWidget::onActivateBillAcceptor(QSharedPointer<Status> status)
{
    _billAcceptorActive = true;
}

void PayPageWidget::onRestartPOS(QSharedPointer<Status> status)
{
    _mainWindow->getMachinery()->deactivatePOS();
}

void PayPageWidget::setPriceLabelsText(QList<QLabel*> labels, const QString& richText)
{
    for (QLabel* label : labels) {
        label->setTextFormat(Qt::RichText);
        label->setText(richText);
    }
}

void PayPageWidget::setPriceToPriceLabels(int price)
{
    QString priceText = QString::number(price);

    setPriceLabelsText(_ui->boldPrices->findChildren<QLabel*>()
                    , QString(_boldPricesText).replace("@PRICE", priceText));
    setPriceLabelsText(_ui->semiBoldPrices->findChildren<QLabel*>()
                    , QString(_semiBoldPricesText).replace("@PRICE", priceText));
    setPriceLabelsText(_ui->regularPrices->findChildren<QLabel*>()
                    , QString(_regularPricesText).replace("@PRICE", priceText));
    setPriceLabelsText(_ui->lightPrices->findChildren<QLabel*>()
                    , QString(_lightPricesText).replace("@PRICE", priceText));
}

void PayPageWidget::setInactionTimer(const QString& durationName)
{
    stopTimer();

    int timeMs = _mainWindow->getConfigManager()->getTimeDuration(getName(), durationName) * 1000;

    _timer.setInterval(timeMs);

    QObject::disconnect(&_timer, &QTimer::timeout, 0, 0);
    QObject::connect(&_timer, &QTimer::timeout, [this]{
        _timer.stop();

        StateName state = _mainWindow->getCurrentStateName();

        if (_enteredMoneyAmount == 0) {
            switch (state) {
                case PAY:
                    _mainWindow->goToState(SPLASH_SCREEN);
                break;
                default:
                    // TODO: handle
                    break;
            }
            return;
        }

        if (_enteredMoneyAmount > 0 && _enteredMoneyAmount < _price) {
            if (state == PAY) {
                _mainWindow->goToState(NOT_ENOUGH_MONEY);
            }

            switch (state) {
                case PAY:
                    _mainWindow->goToState(NOT_ENOUGH_MONEY);
                    break;
                case NOT_ENOUGH_MONEY:
                    _mainWindow->goToState(SPLASH_SCREEN);
                    break;
                default:
                    // TODO: handle
                    break;
            }

            return;
        }

        if (_enteredMoneyAmount >= _price) {
            switch (state) {
                case MORE_MONEY_THAN_NEED:
                    _mainWindow->goToState(PAYMENT_CONFIRMED);
                    break;
                case PAYMENT_CONFIRMED:
                    _mainWindow->goToState(ALCOTEST);
                    break;
                default:
                    // TODO: handle
                    break;
            }
        }
    });

    startTimer(durationName, timeMs / 1000);
}

void PayPageWidget::setNotEnoughMoneyInactionTimer()
{
    stopTimer();

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
            stopTimer();
            _mainWindow->goToState(SPLASH_SCREEN);
        }
    });

    startTimer("inactionNotEnoughMoney", timeMs / 1000);
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

        if (t.getName() == "ofPrice") {
            _ofPriceText = t.getText();
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
