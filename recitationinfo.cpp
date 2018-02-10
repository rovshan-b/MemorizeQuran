#include "recitationinfo.h"

RecitationInfo::RecitationInfo()
{

}

RecitationInfo::RecitationInfo(int index, const QString &title, const QString &baseUrl, const QString &localDirName)
{
    this->index = index;
    this->title = title;
    this->baseUrl = baseUrl;
    this->localDirName = localDirName;
}
