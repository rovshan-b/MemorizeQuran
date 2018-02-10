#ifndef RECITATIONINFO_H
#define RECITATIONINFO_H

#include <QString>
#include <QMetaType>

class RecitationInfo
{
public:
    RecitationInfo();
    RecitationInfo(int index, const QString &title, const QString &baseUrl, const QString &localDirName);

    int index;
    QString title;
    QString baseUrl;
    QString localDirName;
};

Q_DECLARE_METATYPE(RecitationInfo*)

#endif // RECITATIONINFO_H
