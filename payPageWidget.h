#pragma once

#include <QWidget>
#include <QLabel>

#include "page.h"

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

private:
    void initInterface();

    // init column price QLabel's
    // @param labels - column QLabels
    // @param richText - text to set (html text)
    void initPriceLabels(QList<QLabel*> labels, const QString& richText);

    void retrieveTextTemplates();

    Ui::PayPageWidget* _ui;

    //pay complete subpage price text templates
    QString _boldPricesText; // 1 column
    QString _semiBoldPricesText; // 2 column
    QString _regularPricesText; // 3 column
    QString _lightPricesText; // 4 column

    QString _priceText; // template text for QLabel price
};
