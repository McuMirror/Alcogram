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

void PayPageWidget::init(MainWindow *mainWindow) {
    initInterface();

    //_ui->payState->setCurrentIndex(1);
}

void PayPageWidget::initInterface() {
    initPriceLabels(_ui->boldPrices->findChildren<QLabel*>(), "<html>"
                                                              "<head/>"
                                                              "<body>"
                                                              "<p>"
                                                              "<span style=\"font-size:116px; font-weight:600; font-family:Proxima Nova Rg; \">150 </span>"
                                                              "<span style=\"font-size:116px; font-family:.Helvetica Neue Interface; \">₽</span>"
                                                              "</p>"
                                                              "</body>"
                                                              "</html>");

    initPriceLabels(_ui->semiBoldPrices->findChildren<QLabel*>(), "<html>"
                                                                  "<head/>"
                                                                  "<body>"
                                                                  "<p>"
                                                                  "<span style=\"font-size:116px; font-weight:600; font-family:Proxima Nova Lt; \">150 </span>"
                                                                  "<span style=\"font-size:116px; font-family:.Helvetica Neue Interface; \">₽</span>"
                                                                  "</p>"
                                                                  "</body>"
                                                                  "</html>");

    initPriceLabels(_ui->regularPrices->findChildren<QLabel*>(), "<html>"
                                                                 "<head/>"
                                                                 "<body>"
                                                                 "<p>"
                                                                 "<span style=\"font-size:116px; font-family:Proxima Nova Rg; \">150 </span>"
                                                                 "<span style=\"font-size:116px; font-family:.Helvetica Neue Interface; \">₽</span>"
                                                                 "</p>"
                                                                 "</body>"
                                                                 "</html>");

    initPriceLabels(_ui->lightPrices->findChildren<QLabel*>(), "<html>"
                                                               "<head/>"
                                                               "<body>"
                                                               "<p>"
                                                               "<span style=\"font-size:116px; font-family:Proxima Nova Lt; \">150 </span>"
                                                               "<span style=\"font-size:116px; font-family:.Helvetica Neue Interface; \">₽</span>"
                                                               "</p>"
                                                               "</body>"
                                                               "</html>");

    //steps text font
    QFont font = Utils::getFont("Proxima Nova Rg", 20, 2, QFont::Bold);

    _ui->stepOne->setFont(font);
    _ui->stepTwo->setFont(font);
    _ui->stepThree->setFont(font);
    _ui->stepFour->setFont(font);

    font = Utils::getFont("Proxima Nova Rg", 52, 0, QFont::Bold);
    _ui->payCompleteText->setFont(font);
    _ui->payDescription->setFont(font);

    _ui->nextStepDescription->setFont(Utils::getFont("Proxima Nova Rg", 32, 0, QFont::Bold));
    _ui->price->setFont(Utils::getFont("Proxima Nova Rg", 104, 0, QFont::Bold));
}

void PayPageWidget::initPriceLabels(QList<QLabel*> labels, const QString& richText) {
    for (QLabel* label : labels) {
        label->setTextFormat(Qt::RichText);
        label->setText(richText);
    }
}
