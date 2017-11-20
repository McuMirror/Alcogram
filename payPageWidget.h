#pragma once

#include <QWidget>
#include <QLabel>
#include <QTimer>

#include "page.h"
#include "deviceInterfaces/posinterface.h"

namespace Ui {
class PayPageWidget;
}

class PayPageWidget : public Page
{
    Q_OBJECT

public:
    explicit PayPageWidget(QWidget *parent = 0);
    ~PayPageWidget();

    void init(MainWindow* mainWindow) override;
    QString getName() const override;
    QList<Transition*> getTransitions() override;
    void onEntry() override;

protected:
    void initInterface() override;
    void setConnections() override;

private:
    // init column price QLabel's
    // @param labels - column QLabels
    // @param richText - text to set (html text)
    void setPriceLabelsText(QList<QLabel*> labels, const QString& richText);

    void setPriceToPriceLabels(int price);

    // setting inaction timer with duration named durationName
    void setInactionTimer(const QString& durationName);

    void setNotEnoughMoneyInactionTimer();

    void retrieveTextTemplates();
    void retrieveLanguageDependentTemplates();

    // returns payState page numbers related to specific state
    QMap<StateName, int> initPayStatePageNumbers() const;

    // setting page for payState by stateName
    void setSubPage(StateName stateName);

    Ui::PayPageWidget* _ui;

    //pay complete subpage price text templates
    QString _boldPricesText; // 1 column
    QString _semiBoldPricesText; // 2 column
    QString _regularPricesText; // 3 column
    QString _lightPricesText; // 4 column

    QString _priceText; // template text for QLabel price
    QString _timerText; // template for NOT_ENOUGH_MONEY timer text

    QTimer _timer;
    POSInterface* _posDevice;
    int _enteredMoneyAmount;
    int _price;
    int _timerTimeLeft;

    QMap<StateName, int> _payStatePageNumber; // state -> payState page number
};
