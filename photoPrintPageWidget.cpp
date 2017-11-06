#include <QGraphicsDropShadowEffect>

#include "photoPrintPageWidget.h"
#include "ui_photoPrintPageWidget.h"
#include "utils.h"

PhotoPrintPageWidget::PhotoPrintPageWidget(QWidget *parent) :
    Page(parent),
    _ui(new Ui::PhotoPrintPageWidget)
{
    _ui->setupUi(this);
}

PhotoPrintPageWidget::~PhotoPrintPageWidget()
{
    delete _ui;
}

void PhotoPrintPageWidget::init(MainWindow *mainWindow)
{
    initInterface();
    _ui->printPages->setCurrentIndex(1);
}


void PhotoPrintPageWidget::initInterface()
{
    //preview shadow
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);

    effect->setBlurRadius(128);
    effect->setColor(QColor(0, 0, 0, 127));
    effect->setOffset(0, 42);

    _ui->preview->setGraphicsEffect(effect);

    //steps text font
    QFont font = Utils::getFont("Proxima Nova Rg", 20, 2, QFont::Bold);

    _ui->stepOne->setFont(font);
    _ui->stepTwo->setFont(font);
    _ui->stepThree->setFont(font);
    _ui->stepFour->setFont(font);

    font = Utils::getFont("Proxima Nova Rg", 52, 0, QFont::Bold);
    _ui->printingText->setFont(font);
    _ui->shareText1->setFont(font);

    _ui->shareText2->setFont(Utils::getFont("Proxima Nova Rg", 32, 0, QFont::Bold));

    _ui->shareText3->setTextFormat(Qt::RichText);
    _ui->shareText3->setText("<html>"
                             "<head/>"
                             "<body>"
                             "<p>"
                             "<span style=\"font-size:32px; font-weight:600; font-family:Proxima Nova Rg; \">В подарок — </span>"
                             "<span style=\"font-size:52px; font-weight:600; font-family:Proxima Nova Rg; \">5%</span>"
                             "<span style=\"font-size:32px; font-weight:600; font-family:Proxima Nova Rg; \"> на коктейли в баре :)</span>"
                             "</p>"
                             "</body>"
                             "</html>");
}
