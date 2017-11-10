#include <QLabel>
#include <QPushButton>

#include "page.h"
#include "utils.h"

void Page::setTexts(QWidget* mainWidget, const QList<Text>& texts) {
    for (const Text t : texts) {
        QWidget* w = mainWidget->findChild<QWidget*>(t.name);

        if (QLabel* l = dynamic_cast<QLabel*>(w)) {
            if (t.size != 0) {
                l->setFont(Utils::getFont(t.fontFamily, t.size, t.spacing, t.weight));
            } else {
                l->setTextFormat(Qt::RichText);
            }

            l->setText(t.text);
        }

        if (QPushButton* b = dynamic_cast<QPushButton*>(w)) {
            if (t.size != 0) {
                b->setFont(Utils::getFont(t.fontFamily, t.size, t.spacing, t.weight));
            }

            b->setText(t.text);
        }
    }
}


void Page::updateTexts(QWidget* mainWidget, bool onlyLanguageDependent) {
    QString pageName = getName();

    ConfigManager* configManager = _mainWindow->getConfigManager();

    setTexts(mainWidget, configManager->getTexts(pageName));

    if (!onlyLanguageDependent) {
        setTexts(mainWidget, configManager->getLanguageIndependentText(pageName));
    }
}
