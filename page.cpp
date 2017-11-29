#include <QLabel>
#include <QPushButton>
#include <QDebug>

#include "page.h"
#include "utils.h"
#include "logger.h"

void Page::enter() {
    qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::PAGE_ENTRANCE_START, 0, 0
        , QStringList({getName()}));

    onEntry();

    qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::PAGE_ENTRANCE_END, 0, 0
        , QStringList({getName()}));

}

void Page::setTexts(QWidget* mainWidget, const QList<Text>& texts) {
    for (const Text t : texts) {
        QWidget* w = mainWidget->findChild<QWidget*>(t.getName());

        if (QLabel* l = dynamic_cast<QLabel*>(w)) {
            if (t.getSize() != 0) {
                // plain text
                l->setFont(Utils::getFont(t.getFontFamily(), t.getSize()
                                          , t.getSpacing(), t.getWeight()));
            } else {
                // html text
                l->setTextFormat(Qt::RichText);
            }

            l->setText(t.getText());
        }

        if (QPushButton* b = dynamic_cast<QPushButton*>(w)) {
            if (t.getSize() != 0) {
                b->setFont(Utils::getFont(t.getFontFamily(), t.getSize()
                                          , t.getSpacing(), t.getWeight()));
            }

            b->setText(t.getText());
        }
    }
}

void Page::startTimer(const QString& timerName, int timeS)
{
    _timer.start();
    _currentTimerName = timerName;

    qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::USER_TIMER_START, Logger::NONE
        , _currentTimerName, 0, 0, QList<double>({timeS}));
}

void Page::stopTimer()
{
    _timer.stop();

    if (!_currentTimerName.isEmpty()) {
        qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::USER_TIMER_STOP, Logger::NONE
            , _currentTimerName);
    }

    _currentTimerName = "";
}


void Page::updateTexts(QWidget* mainWidget, bool onlyLanguageDependent) {
    QString pageName = getName();

    ConfigManager* configManager = _mainWindow->getConfigManager();

    setTexts(mainWidget, configManager->getTexts(pageName));

    if (!onlyLanguageDependent) {
        setTexts(mainWidget, configManager->getLanguageIndependentText(pageName));
    }
}
