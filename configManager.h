#pragma once

#include <QObject>
#include <QXmlStreamReader>
#include <QMap>
#include <QList>
#include <QDomNode>

// stores interface element text
class Text {

public:
    explicit Text(const QString& name, const QString& fontFamily
            , int size, qreal spacing, int weight, const QString& text);

    QString getName() const;
    QString getFontFamily() const;
    int getSize() const;
    qreal getSpacing() const;
    int getWeight() const;
    QString getText() const;

private:
    QString _name; // name of interface element (eg QLabel, QPushButton)
    QString _fontFamily;
    int _size;
    qreal _spacing;
    int _weight; // thickness (enum QFont::Weight)
    QString _text;
};

// stores application configurations
class ConfigManager : public QObject
{
    Q_OBJECT
public:
    explicit ConfigManager(QObject *parent = 0);

    // load configuration file
    void load(const QString& fileName);

    QString getCurrentLanguage() const;

    // get language dependent text
    const QList<Text> getTexts(const QString& page) const;

    const QList<Text> getLanguageIndependentText(const QString& page) const;

private:
    // parse <page> tag
    void parsePages(QDomNode page);

    // parse <texts> tag
    void parseTexts(QDomNode language, const QString& pageName);

    QMap<QString, QList<Text>> _pageText; // language independent text
    QMap<QString, QList<Text>> _pageTextRU; // russian text
    QMap<QString, QList<Text>> _pageTextEN; // english text
    QString _currentLanguage;
};
