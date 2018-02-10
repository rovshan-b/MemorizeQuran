#ifndef QURANINFO_H
#define QURANINFO_H

class QuranTextInfo;
class SuraInfo;
class RecitationInfo;

#include <QList>
#include <QMap>
#include <QStringList>

class QuranInfo
{
public:
    QuranInfo();
    virtual ~QuranInfo();

    QList<QuranTextInfo*> texts();
    QuranTextInfo *text(int index) const; //1 based index

    QList<SuraInfo*> suras() const;
    SuraInfo *sura(int index) const; //1 based index

    QList<QuranTextInfo*> translations();
    QuranTextInfo *translation(int index) const; //1 based index

    QList<RecitationInfo*> recitations();
    RecitationInfo *recitation(int index) const; //1 based index

    void loadAyas(QuranTextInfo *textInfo);
    void loadTranslations(QuranTextInfo *textInfo);

private:
    QList<QuranTextInfo*> m_texts;
    QList<QuranTextInfo*> m_translations;
    QList<RecitationInfo*> m_recitations;
    QList<SuraInfo*> m_suras;

    void loadSuras();
};

#endif // QURANINFO_H
