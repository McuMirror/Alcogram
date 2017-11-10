#pragma once

#include <QObject>
#include <QXmlStreamReader>
#include <QMap>
#include <QList>
#include <QDomNode>

class Text {
public:
    QString name;
    QString fontFamily;
    int size;
    qreal spacing;
    int weight;
    QString text;
};

class ConfigManager : public QObject
{
    Q_OBJECT
public:
    explicit ConfigManager(QObject *parent = 0);

    void load(const QString& fileName);

    QString getCurrentLanguage() const;
    const QList<Text> getTexts(const QString& page) const;
    const QList<Text> getLanguageIndependentText(const QString& page) const;

private:
    void parsePages(QDomNode page);
    void parseTexts(QDomNode language, const QString& pageName);

    QMap<QString, QList<Text>> _pageText; // language independent text
    QMap<QString, QList<Text>> _pageTextRU; // russian text
    QMap<QString, QList<Text>> _pageTextEN; // english text
    QString _currentLanguage;
};
