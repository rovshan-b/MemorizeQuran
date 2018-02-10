#ifndef CACHINGPLAYER_H
#define CACHINGPLAYER_H

#include <QObject>
#include <QWidget>
#include <QNetworkAccessManager>
#include "downloadmanager.h"

class AudioPlayer;
class QProgressBar;
class SuraInfo;
class QStackedLayout;

class CachingPlayer : public QWidget
{
    Q_OBJECT
public:
    explicit CachingPlayer(QWidget *parent = 0);

    void setCurrentAya(SuraInfo *sura, int aya);

signals:
    void next();
    void previous();

private slots:
    void startDownload();

    void dlProgress(quint64 read, quint64 total);
    void dlError(const QString &message);
    void dlFinished();

private:
    QStackedLayout *m_mainLayout;

    AudioPlayer *m_player;
    QProgressBar *m_progressBar;

    SuraInfo *m_sura;
    int m_aya;

    QString m_currentFilename;
    QString m_currentUrl;

    QNetworkAccessManager m_networkManager;

    DownloadManager m_downloadManager;
};

#endif // CACHINGPLAYER_H
