#include "criticalErrorPage.h"
#include "ui_criticalErrorPage.h"

CriticalErrorPage::CriticalErrorPage(QWidget *parent) :
    Page(parent),
    ui(new Ui::CriticalErrorPage)
{
    ui->setupUi(this);
}

CriticalErrorPage::~CriticalErrorPage()
{
    delete ui;
}

void CriticalErrorPage::init(MainWindow *mainWindow)
{
    Page::init(mainWindow);
}

QString CriticalErrorPage::getName() const
{
    return "criticalError";
}
