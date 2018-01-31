#ifndef QURANTEXTINFO_H
#define QURANTEXTINFO_H

#include <QString>
#include <QMetaType>

class QuranTextInfo
{
public:
    QuranTextInfo();
    QuranTextInfo(int index, const QString &title, const QString &filename);

    int index;
    QString title;
    QString filename;
};

Q_DECLARE_METATYPE(QuranTextInfo*)

#endif // QURANTEXTINFO_H
