#ifndef SURAINFO_H
#define SURAINFO_H

#include <QString>
#include <QStringList>
#include <QMetaType>

class SuraInfo
{
public:
    SuraInfo();
    SuraInfo(int index, int ayas, int start, const QString &name, const QString &tname, const QString &ename, const QString &type);

    int index;
    int ayas;
    int start;
    QString name;
    QString tname;
    QString ename;
    QString type;

    void addAya(const QString &aya);
    void clearAyas();
    QString ayaText(int index) const;

    void addTranslation(const QString &translationText);
    void clearTranslations();
    QString translationText(int index) const;

private:
    QStringList m_ayaTexts;
    QStringList m_translationTexts;
};

Q_DECLARE_METATYPE(SuraInfo*)

#endif // SURAINFO_H
