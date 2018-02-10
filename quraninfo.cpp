#include "quraninfo.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include "qurantextinfo.h"
#include "surainfo.h"

QuranInfo::QuranInfo()
{
    m_texts.append(new QuranTextInfo(1, "Simple enhanced", "quran-simple-enhanced.txt"));
    m_texts.append(new QuranTextInfo(2, "Uthmani", "quran-uthmani.txt"));

    m_translations.append(new QuranTextInfo(1, "None", ""));
    m_translations.append(new QuranTextInfo(2, "English - Saheeh International", "en.sahih.txt"));
    m_translations.append(new QuranTextInfo(3, "Azerbaijani - Alikhan Musayev", "az.musayev.txt"));
    m_translations.append(new QuranTextInfo(4, "Azerbaijani - Vasim Mammadaliyev and Ziya Bunyadov", "az.mammadaliyev.txt"));
    m_translations.append(new QuranTextInfo(5, "Russian - Elmir Kuliev", "ru.kuliev.txt"));

    loadSuras();
}

QuranInfo::~QuranInfo()
{
    qDeleteAll(m_suras);
    qDeleteAll(m_texts);
    qDeleteAll(m_translations);
}

QList<QuranTextInfo *> QuranInfo::texts()
{
    return this->m_texts;
}

QuranTextInfo *QuranInfo::text(int index) const
{
    return m_texts.at(index - 1);
}

SuraInfo *QuranInfo::sura(int index) const
{
    return m_suras.at(index - 1);
}

QList<QuranTextInfo *> QuranInfo::translations()
{
    return this->m_translations;
}

QuranTextInfo *QuranInfo::translation(int index) const
{
    return m_translations.at(index - 1);
}

void QuranInfo::loadAyas(QuranTextInfo *textInfo)
{
    QFile textFile(QString(":/quran/%1").arg(textInfo->filename));
    textFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&textFile);
    stream.setCodec("UTF-8");

    foreach (SuraInfo *suraInfo, suras()) {
        suraInfo->clearAyas();
    }

    while (!stream.atEnd()) {
        QStringList parts = stream.readLine().split('|');

        if (parts.count() < 3) {
            continue;
        }

        int suraIx = parts.at(0).toInt();
        QString text = parts.at(2);

        SuraInfo *suraInfo = sura(suraIx);

        if (suraIx != 1 && suraIx != 9 && suraInfo->ayaTextCount() == 0) {
            QStringList words = text.split(' ');
            if (words.count() > 4) {
                words.insert(4, "\n");
            }
            text = words.join(" ");
        }

        suraInfo->addAya(text);
    }
}

void QuranInfo::loadTranslations(QuranTextInfo *textInfo)
{
    foreach (SuraInfo *suraInfo, suras()) {
        suraInfo->clearTranslations();
    }

    if (textInfo->filename.isEmpty()) {
        return;
    }

    QFile textFile(QString(":/quran/%1").arg(textInfo->filename));
    textFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&textFile);
    stream.setCodec("UTF-8");

    while (!stream.atEnd()) {
        QStringList parts = stream.readLine().split('|');

        if (parts.count() < 3) {
            continue;
        }

        int suraIx = parts.at(0).toInt();
        QString text = parts.at(2);

        sura(suraIx)->addTranslation(text);
    }
}

QList<SuraInfo *> QuranInfo::suras() const
{
    return m_suras;
}

void QuranInfo::loadSuras()
{
    QFile surasFile(":/quran/suras.txt");
    surasFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&surasFile);
    QString line;

    m_suras.reserve(114);

    stream.readLine(); //first line is header

    while (!stream.atEnd()) {
        QString line = stream.readLine();
        QStringList parts = line.split('|');
        int index = parts.at(0).toInt();
        int ayas = parts.at(1).toInt();
        int start = parts.at(2).toInt();
        QString name = parts.at(3);
        QString tname = parts.at(4);
        QString ename = parts.at(5);
        QString type = parts.at(6);

        m_suras.append(new SuraInfo(index, ayas, start, name, tname, ename, type));
    }
}
