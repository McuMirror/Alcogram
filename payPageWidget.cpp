#include "payPageWidget.h"
#include "ui_payPageWidget.h"
#include "utils.h"

PayPageWidget::PayPageWidget(QWidget *parent) :
    Page(parent),
    _ui(new Ui::PayPageWidget)
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
    initInterface();

    _ui->payState->setCurrentIndex(1);
}

QString PayPageWidget::getName() const
{
    return "pay";
}

void PayPageWidget::initInterface() {
    updateTexts(_ui->frame);
    retrieveTextTemplates();

    QString price = "150"; //TODO: get rid of it

    initPriceLabels(_ui->boldPrices->findChildren<QLabel*>(), _boldPricesText.replace("@PRICE", price));
    initPriceLabels(_ui->semiBoldPrices->findChildren<QLabel*>(), _semiBoldPricesText.replace("@PRICE", price));
    initPriceLabels(_ui->regularPrices->findChildren<QLabel*>(), _regularPricesText.replace("@PRICE", price));
    initPriceLabels(_ui->lightPrices->findChildren<QLabel*>(), _lightPricesText.replace("@PRICE", price));

    _ui->price->setText(_priceText.replace("@PRICE", price));
}

void PayPageWidget::initPriceLabels(QList<QLabel*> labels, const QString& richText)
{
    for (QLabel* label : labels) {
        label->setTextFormat(Qt::RichText);
        label->setText(richText);
    }
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

}
