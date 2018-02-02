#include <QFile>
#include <QMap>
#include <QFont>
#include <QDebug>

#include "configManager.h"
#include "logger.h"

//Text

Text::Text()
    : _name("")
    , _fontFamily("Proxima Nova Rg")
    , _size(1)
    , _spacing(0)
    , _weight(QFont::Normal)
    , _text("")
{

}

Text::Text(const QString &name, const QString &fontFamily, int size
           , qreal spacing, int weight, const QString &text)
    : _name(name)
    , _fontFamily(fontFamily)
    , _size(size)
    , _spacing(spacing)
    , _weight(weight)
    , _text(text)
{
}

QString Text::getName() const
{
    return _name;
}

QString Text::getFontFamily() const
{
    return _fontFamily;
}

int Text::getSize() const
{
    return _size;
}

qreal Text::getSpacing() const
{
    return _spacing;
}

int Text::getWeight() const
{
    return _weight;
}

QString Text::getText() const
{
    return _text;
}

// Interval

Interval::Interval()
{

}

Interval::Interval(float start, float stop)
    : _start(start)
    , _stop(stop)
    , _startNotLimited(false)
    , _stopNotLimited(false)
{

}

float Interval::getStart() const
{
    return _start;
}

float Interval::getStop() const
{
    return _stop;
}

bool Interval::isStartNotLimited() const
{
    return _start == -1;
}

// Config Manager

const QString ConfigManager::_requestNameStrings[] = {"startDevice", "finishDevice", "restartDevice", "checkStatus", "checkConnection"
                                               , "getImage", "takeImage", "stopGetImage" , "warmingUpAlcotester", "coolingDownAlcotester"
                                               , "activateAlcotester", "activatePOS", "deactivatePOS", "takeMoney"
                                               , "warmingUpPrinter", "coolingDownPrinter", "printImage"
                                               , "activateBillAcceptor", "deactivateBillAcceptor"};

ConfigManager::ConfigManager(QObject *parent)
    : QObject(parent)
    , _currentLanguage(RU)
    , _defaultInterval(std::numeric_limits<float>::min(), std::numeric_limits<float>::max())
    , _defaultGradient(QColor(Qt::white), QColor(Qt::black))
{
    for (int i = START_DEVICE; i <= DEACTIVATE_BILL_ACCEPTOR; i++) {
        _requestNames.insert(_requestNameStrings[i], static_cast<RequestName>(i));
    }
}

ConfigManager::TextLanguage ConfigManager::getCurrentLanguage() const
{
    return _currentLanguage;
}

void ConfigManager::switchLanguage()
{
    if (_currentLanguage == RU) {
        _currentLanguage = EN;
    } else {
        _currentLanguage = RU;
    }
}

const QList<Text> ConfigManager::getTexts(const QString& page) const
{
    switch (_currentLanguage) {
        case RU:
            if (_pageTextRU.contains(page)) {
                return _pageTextRU[page].values();
            }
        break;
        case EN:
            if (_pageTextEN.contains(page)) {
                return _pageTextEN[page].values();
            }
            break;
    }

    // TODO: logging, language dependent page text not found
    return QList<Text>();
}

Text ConfigManager::getText(const QString& pageName, const QString& textName, TextLanguage language) const
{
    switch (language) {
        case NOT_SPECIFIED:
            return getPageText(_pageText, pageName, textName);
        case RU:
            return getPageText(_pageTextRU, pageName, textName);
        case EN:
            return getPageText(_pageTextEN, pageName, textName);
        case CURRENT:
            return getText(pageName, textName, _currentLanguage);
    }

    return _defaultText;
}

Text ConfigManager::getPageText(const QMap<QString, QMap<QString, Text> >& pageTexts, const QString& pageName
                                   , const QString& textName) const
{
    if (pageTexts.contains(pageName)) {
        if (pageTexts[pageName].contains(textName)) {
            return pageTexts[pageName].value(textName);
        }
    }

    return _defaultText;
}

const QList<Text> ConfigManager::getLanguageIndependentText(const QString& page) const
{
    if (_pageText.contains(page)) {
        return _pageText[page].values();
    }

    // TODO: logging, language independent page text not found

    return QList<Text>();
}

int ConfigManager::getTimeDuration(const QString& pageName, const QString& durationName) const
{
    if (_durations.contains(pageName)) {
        QMap<QString, int> pageDurations = _durations[pageName];

        if (pageDurations.contains(durationName)) {
            return pageDurations[durationName];
        }

        // TODO: logging, duration name not found

        return 0;
    }

    // TODO: logging, page not found

    return 0;
}

int ConfigManager::getTimeDuration(DeviceName deviceName, RequestName requestName) const
{
    return _requestTimeouts[deviceName][requestName];
}

QPair<float, float> ConfigManager::getInterval(const QString& pageName, const QString& intervalName) const
{
    if (_intervals.contains(pageName)) {
        if (_intervals[pageName].contains(intervalName)) {
            return _intervals[pageName].value(intervalName);
        }
    }

    return _defaultInterval;
}

QPair<QColor, QColor> ConfigManager::getGradient(const QString& pageName, const QString& gradientName) const
{
    if (_gradients.contains(pageName)) {
        if (_gradients[pageName].contains(gradientName)) {
            return _gradients[pageName].value(gradientName);
        }
    }

    return _defaultGradient;
}

double ConfigManager::getSize(const QString& pageName, const QString& sizeName) const
{
    if (_pageSizes.contains(pageName)) {
        if (_pageSizes[pageName].contains(sizeName)) {
            return _pageSizes[pageName].value(sizeName);
        }
    }

    return 0;
}

void ConfigManager::load(const QString& fileName)
{
    qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::CONFIG_LOAD_START, 0, 0, QStringList({fileName}));

    QFile* file = new QFile(fileName);

    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        // TODO: logging
        return;
    }

    QDomDocument domDocument;

    QString errorMessage;
    int errorLine;
    int errorColumn;

    bool error = !domDocument.setContent(file, &errorMessage, &errorLine, &errorColumn);

    if (error) {
        //  TODO: logging, errors described in QXmlParseException class documentation
        return;
    }


    QDomElement topElement = domDocument.documentElement();
    QDomNode domNode = topElement.firstChild();

    while (!domNode.isNull()) {
        QDomElement domElement = domNode.toElement();

        if (domElement.tagName() == "pages") {
            parsePages(domElement.firstChild());
        }

        if (domElement.tagName() == "devices") {
            parseDevices(domElement.firstChild());
        }

        domNode = domNode.nextSibling();
    }

    delete file;

    qDebug().noquote() << Logger::instance()->buildSystemEventLog(Logger::CONFIG_LOAD_FINISH);
}

void ConfigManager::parseDevices(QDomNode device)
{
    // iterate over <device> tags

    while(!device.isNull()) {
        QDomElement deviceElement = device.toElement();
        QString name = deviceElement.attribute("name", "");
        DeviceName deviceName = getDeviceName(name);

        _requestTimeouts.insert(deviceName, QMap<RequestName, int>());

        QDomNode deviceSet = device.firstChild();

        while (!deviceSet.isNull()) {
            QDomElement deviceSetElement = deviceSet.toElement();

            if (deviceSetElement.tagName() == "timeouts") {
                parseTimeouts(deviceSetElement.firstChild(), deviceName);
            }

            deviceSet = deviceSet.nextSibling();
        }

        device = device.nextSibling();
    }
}

void ConfigManager::parseTimeouts(QDomNode timeout, DeviceName deviceName)
{
    // iterate over <timeout> tags
    while (!timeout.isNull()) {
        QDomElement timeoutElement = timeout.toElement();
        QString timeoutName = timeoutElement.attribute("name", "");
        RequestName requestName = _requestNames[timeoutName];
        QString dimension = timeoutElement.attribute("dimension", "ms");
        int time = timeoutElement.attribute("value", "0").toInt();

        if (dimension == "s") {
            time *= 1000;
        } else if (dimension == "min") {
            time *= 60000;
        }

        _requestTimeouts[deviceName].insert(requestName, time);

        timeout = timeout.nextSibling();
    }
}

DeviceName ConfigManager::getDeviceName(const QString &name) const
{
    if (name == "camera") {
        return CAMERA;
    }

    if (name == "pos") {
        return POS;
    }

    if (name == "alcotester") {
        return ALCOTESTER;
    }

    if (name == "printer") {
        return PRINTER;
    }

    return NON_DEVICE;
}

void ConfigManager::parsePages(QDomNode page)
{
    // iterate over <page> tags
    while (!page.isNull()) {
        QDomElement pageElement = page.toElement();
        QString pageName = pageElement.attribute("name", "");

        _pageTextRU.insert(pageName, QMap<QString, Text>());
        _pageTextEN.insert(pageName, QMap<QString, Text>());
        _pageText.insert(pageName, QMap<QString, Text>());
        _pageSizes.insert(pageName, QMap<QString, double>());

        QDomNode pageSet = page.firstChild();

        // iterate over page settings tags
        while (!pageSet.isNull()) {
            QDomElement pageSetElement = pageSet.toElement();

            if (pageSetElement.tagName() == "texts") {
                parseTexts(pageSetElement.firstChild(), pageName);
            }

            if (pageSetElement.tagName() == "durations") {
                parseDurations(pageSetElement.firstChild(), pageName);
            }

            if (pageSetElement.tagName() == "intervals") {
                parseIntervals(pageSetElement.firstChild(), pageName);
            }

            if (pageSetElement.tagName() == "colors") {
                parseColors(pageSetElement.firstChild(), pageName);
            }

            if (pageSetElement.tagName() == "sizes") {
                parseSizes(pageSetElement.firstChild(), pageName);
            }

            pageSet = pageSet.nextSibling();
        }

        page = page.nextSibling();
    }
}

void ConfigManager::parseSizes(QDomNode size, const QString& pageName)
{
    // iterate over <size> tags
    while (!size.isNull()) {
        QDomElement sizeElement = size.toElement();
        QString sizeName = sizeElement.attribute("name", "");
        int iSize = sizeElement.attribute("value", "0").toInt();
        QString dimension = sizeElement.attribute("dimension", "px");
        double dSize = dimension == "%" ? iSize / 100.0 : iSize;

        _pageSizes[pageName].insert(sizeName, dSize);

        size = size.nextSibling();
    }
}

void ConfigManager::parseTexts(QDomNode language, const QString& pageName)
{
    // iterate over <language> tags
    while (!language.isNull()) {
        QDomElement languageElement = language.toElement();
        QString languageName = languageElement.attribute("name", "");
        QDomNode text = languageElement.firstChild();

        // iterate over <text> tags
        while (!text.isNull()) {
            QDomElement textElement = text.toElement();

            Text t = Text(textElement.attribute("name", "")
                          , textElement.attribute("fontFamily", "Proxima Nova Rg")
                          , textElement.attribute("size", "1").toInt()
                          , textElement.attribute("spacing", "0").toDouble()
                          , textElement.attribute("weight", "75").toInt()
                          , textElement.text().remove(QRegExp("[\\n\\t\\r]")).simplified());

            if (languageName == "RU") {
                _pageTextRU[pageName].insert(t.getName(), t);
            } else if (languageName == "EN") {
                _pageTextEN[pageName].insert(t.getName(), t);
            } else {
                _pageText[pageName].insert(t.getName(), t);
            }

            text = text.nextSibling();
        }

        language = language.nextSibling();
    }
}

void ConfigManager::parseDurations(QDomNode duration, const QString &pageName)
{
    // iterate over <duration> tags
    while (!duration.isNull()) {
        QDomElement durationElement = duration.toElement();
        QString durationName = durationElement.attribute("name", "");
        int time = durationElement.attribute("value", "0").toInt();

        if (!_durations.contains(pageName)) {
            _durations.insert(pageName, QMap<QString, int>());
        }

        _durations[pageName].insert(durationName, time);

        duration = duration.nextSibling();
    }
}

void ConfigManager::parseIntervals(QDomNode interval, const QString& pageName)
{
    // iterate over <interval> tags
    while (!interval.isNull()) {
        QDomElement intervalElement = interval.toElement();
        QString intervalName = intervalElement.attribute("name", "");
        QString start = intervalElement.attribute("start", "");
        QString stop = intervalElement.attribute("stop", "");
        bool ok = true;
        float startValue = start.isEmpty() ? std::numeric_limits<float>::min() : start.toFloat(&ok);

        if (!ok) {
            startValue = std::numeric_limits<float>::min();
        }

        float stopValue = stop.isEmpty() ? std::numeric_limits<float>::max() : stop.toFloat(&ok);

        if (!ok) {
            stopValue = std::numeric_limits<float>::max();
        }

        if (!_intervals.contains(pageName)) {
            _intervals.insert(pageName, QMap<QString, QPair<float, float>>());
        }

        _intervals[pageName].insert(intervalName, QPair<float, float>(startValue, stopValue));

        interval = interval.nextSibling();
    }
}

void ConfigManager::parseColors(QDomNode color, const QString& pageName)
{
    while (!color.isNull()) {
        QDomElement colorElement = color.toElement();

        if (colorElement.tagName() == "gradient") {
            QString gradientName = colorElement.attribute("name", "");
            QString color1 = colorElement.attribute("color1", "");
            QString color2 = colorElement.attribute("color2", "");

            if (!_gradients.contains(pageName)) {
                _gradients.insert(pageName, QMap<QString, QPair<QColor, QColor>>());
            }
            _gradients[pageName].insert(gradientName, QPair<QColor, QColor>(QColor(color1), QColor(color2)));
        }

        color = color.nextSibling();
    }
}
