#include <QFile>
#include <QMap>
#include "configManager.h"

ConfigManager::ConfigManager(QObject *parent)
    : QObject(parent)
    , _currentLanguage("RU")
{

}

QString ConfigManager::getCurrentLanguage() const
{
    return _currentLanguage;
}

const QList<Text> ConfigManager::getTexts(const QString& page) const
{
    if (_currentLanguage == "RU") {
        if (_pageTextRU.contains(page)) {
            return _pageTextRU[page];
        }
    }

    if (_currentLanguage == "EN") {
        if (_pageTextEN.contains(page)) {
            return _pageTextEN[page];
        }
    }

    return QList<Text>();
}

const QList<Text> ConfigManager::getLanguageIndependentText(const QString& page) const
{
    if (_pageText.contains(page)) {
        return _pageText[page];
    }

    return QList<Text>();
}

void ConfigManager::load(const QString& fileName)
{
    QFile* file = new QFile(fileName);

    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        // TODO: logging
        return;
    }

    QDomDocument domDocument;
    domDocument.setContent(file);
    QDomElement topElement = domDocument.documentElement();
    QDomNode domNode = topElement.firstChild();

    while (!domNode.isNull()) {
        QDomElement domElement = domNode.toElement();

        if (domElement.tagName() == "pages") {
            parsePages(domElement.firstChild());
        }

        domNode = domNode.nextSibling();
    }
}

void ConfigManager::parsePages(QDomNode page)
{
    while (!page.isNull()) {
        QDomElement pageElement = page.toElement();
        QString pageName = pageElement.attribute("name", "");

        _pageTextRU.insert(pageName, QList<Text>());
        _pageTextEN.insert(pageName, QList<Text>());
        _pageText.insert(pageName, QList<Text>());

        QDomNode pageSet = page.firstChild();

        //iterate by page settings
        while (!pageSet.isNull()) {
            QDomElement pageSetElement = pageSet.toElement();

            if (pageSetElement.tagName() == "texts") {
                parseTexts(pageSetElement.firstChild(), pageName);
            }

            pageSet = pageSet.nextSibling();
        }

        page = page.nextSibling();
    }
}

void ConfigManager::parseTexts(QDomNode language, const QString& pageName)
{
    while (!language.isNull()) {
        QDomElement languageElement = language.toElement();
        QString languageName = languageElement.attribute("name", "");
        QDomNode text = languageElement.firstChild();

        while (!text.isNull()) {
            QDomElement textElement = text.toElement();

            Text t = Text();

            t.name = textElement.attribute("name", "");
            t.fontFamily = textElement.attribute("fontFamily", "Proxima Nova Rg");
            t.size = textElement.attribute("size", "0").toInt();
            t.spacing = textElement.attribute("spacing", "0").toDouble();
            t.weight = textElement.attribute("weight", "75").toInt();
            t.text = textElement.text().remove(QRegExp("[\\n\\t\\r]")).simplified();

            if (languageName == "RU") {
                _pageTextRU[pageName].append(t);
            } else if (languageName == "EN") {
                _pageTextEN[pageName].append(t);
            } else {
                _pageText[pageName].append(t);
            }

            text = text.nextSibling();
        }

        language = language.nextSibling();
    }
}
