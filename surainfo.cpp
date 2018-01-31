#include "surainfo.h"

SuraInfo::SuraInfo()
{

}

SuraInfo::SuraInfo(int index, int ayas, int start, const QString &name, const QString &tname, const QString &ename, const QString &type)
{
    this->index = index;
    this->ayas = ayas;
    this->start = start;
    this->name = name;
    this->tname = tname;
    this->ename = ename;
    this->type = type;
}

void SuraInfo::addAya(const QString &aya)
{
    if (m_ayaTexts.empty()) {
        m_ayaTexts.reserve(this->ayas);
    }

    m_ayaTexts.append(aya);
}

void SuraInfo::clearAyas()
{
    m_ayaTexts.clear();
}

QString SuraInfo::ayaText(int index) const
{
    int indexToGet = qMin(qMax(index, 1), ayas);

    return m_ayaTexts.at(indexToGet - 1);
}

void SuraInfo::addTranslation(const QString &translation)
{
    if (m_translationTexts.empty()) {
        m_translationTexts.reserve(this->ayas);
    }

    m_translationTexts.append(translation);
}

void SuraInfo::clearTranslations()
{
    m_translationTexts.clear();
}

QString SuraInfo::translationText(int index) const
{
    if (m_translationTexts.count() == 0) {
        return "";
    }

    int indexToGet = qMin(qMax(index, 1), ayas);

    return m_translationTexts.at(indexToGet - 1);
}
