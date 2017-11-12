#include <QLabel>
#include <QPushButton>

#include "page.h"
#include "utils.h"

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


void Page::updateTexts(QWidget* mainWidget, bool onlyLanguageDependent) {
    QString pageName = getName();

    ConfigManager* configManager = _mainWindow->getConfigManager();

    setTexts(mainWidget, configManager->getTexts(pageName));

    if (!onlyLanguageDependent) {
        setTexts(mainWidget, configManager->getLanguageIndependentText(pageName));
    }
}
