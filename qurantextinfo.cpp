#include "qurantextinfo.h"

QuranTextInfo::QuranTextInfo()
{

}

QuranTextInfo::QuranTextInfo(int index, const QString &title, const QString &filename)
{
    this->index = index;
    this->title = title;
    this->filename = filename;
}
