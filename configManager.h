#pragma once

#include <QObject>
#include <QXmlStreamReader>
#include <QMap>
#include <QList>
#include <QDomNode>
#include <QColor>
#include <limits>

#include "deviceInterfaces/status.h"

// stores interface element text
class Text {

public:
    Text();

    Text(const QString& name, const QString& fontFamily
            , int size, qreal spacing, int weight, const QString& text);

    // returns name of text constant (usually name of widget)
    QString getName() const;

    QString getFontFamily() const;

    int getSize() const;

    qreal getSpacing() const;

    int getWeight() const;
    QString getText() const;

private:
    QString _name; // name of interface element (eg QLabel, QPushButton)
    QString _fontFamily;
    int _size; // text size
    qreal _spacing; // spacing between characters
    int _weight; // thickness (enum QFont::Weight)
    QString _text;
};

class Interval {
public:
    Interval();
    Interval(float start, float stop);

    float getStart() const;
    float getStop() const;
    bool isStartNotLimited() const;
    bool isStopNotLimited() const;

private:
    float _start = std::numeric_limits<float>::min();
    float _stop = std::numeric_limits<float>::max();
    bool _startNotLimited = true;
    bool _stopNotLimited = true;
};

// stores application configurations
class ConfigManager : public QObject
{
    Q_OBJECT
public:
    enum TextLanguage {
        NOT_SPECIFIED // language independent
        , RU
        , EN
        , CURRENT // current language
    };

    explicit ConfigManager(QObject *parent = 0);

    // load configuration file
    void load(const QString& fileName);

    TextLanguage getCurrentLanguage() const;

    void switchLanguage();

    // get language dependent text
    const QList<Text> getTexts(const QString& page) const;

    // get Text by page name, text name language
    Text getText(const QString& pageName, const QString& textName, TextLanguage language = CURRENT) const;

    const QList<Text> getLanguageIndependentText(const QString& page) const;

    int getTimeDuration(const QString& pageName, const QString& durationName) const;
    int getTimeDuration(DeviceName deviceName, RequestName requestName) const;

    QPair<float, float> getInterval(const QString& pageName, const QString& intervalName) const;
    QPair<QColor, QColor> getGradient(const QString& pageName, const QString& gradientName) const;

private:
    Text getPageText(const QMap<QString, QMap<QString, Text> > &pageTexts, const QString& pageName
                        , const QString& textName) const;

    // parse <devices> tag
    void parseDevices(QDomNode device);

    // parse <timeout> tag
    void parseTimeouts(QDomNode timeout, DeviceName deviceName);

    DeviceName getDeviceName(const QString& name) const;

    // parse <pages> tag
    void parsePages(QDomNode page);

    // parse <texts> tag
    void parseTexts(QDomNode language, const QString& pageName);

    // parse <durations> tag
    void parseDurations(QDomNode duration, const QString& pageName);

    // parse <intervals> tag
    void parseIntervals(QDomNode interval, const QString& pageName);

    // parse <colors> tag
    void parseColors(QDomNode color, const QString& pageName);

    QMap<QString, QMap<QString, Text>> _pageText; // language independent text
    QMap<QString, QMap<QString, Text>> _pageTextRU; // russian text
    QMap<QString, QMap<QString, Text>> _pageTextEN; // english text
    QMap<QString, QMap<QString, int>> _durations; // page time durations
    QMap<QString, QMap<QString, QPair<float, float>>> _intervals; // page intervals
    QMap<QString, QMap<QString, QPair<QColor, QColor>>> _gradients; // page gradients
    QMap<DeviceName, QMap<RequestName, int>> _requestTimeouts;

    TextLanguage _currentLanguage; // current interface language
    Text _defaultText; // default value for getText() function
    QPair<float, float> _defaultInterval; // default value for getInterval() function
    QPair<QColor, QColor> _defaultGradient; // default value for getGradient() function
    QMap<QString, RequestName> _requestNames;

    static const QString _requestNameStrings[];
};
