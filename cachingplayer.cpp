#include "cachingplayer.h"
#include "audioplayer.h"
#include "surainfo.h"
#include <QtWidgets>
#include <QDebug>

CachingPlayer::CachingPlayer(QWidget *parent) : QWidget(parent),
    m_sura(0),
    m_aya(1),
    m_downloadManager(&m_networkManager)
{
    m_mainLayout = new QStackedLayout;
    m_mainLayout->setContentsMargins(0,0,0,0);

    m_player = new AudioPlayer();
    m_progressBar = new QProgressBar();
    m_progressBar->setRange(0, 100);

    m_mainLayout->addWidget(m_player);
    m_mainLayout->addWidget(m_progressBar);

    setLayout(m_mainLayout);

    connect(m_player, SIGNAL(fileNotFound()), this, SLOT(startDownload()));
    connect(&m_downloadManager, SIGNAL(onProgress(quint64,quint64)), this, SLOT(dlProgress(quint64,quint64)));
    connect(&m_downloadManager, SIGNAL(onError(QString)), this, SLOT(dlError(QString)));
    connect(&m_downloadManager, SIGNAL(onFinished()), this, SLOT(dlFinished()));

    connect(m_player, SIGNAL(next()), this, SIGNAL(next()));
    connect(m_player, SIGNAL(previous()), this, SIGNAL(previous()));
}

void CachingPlayer::setCurrentAya(SuraInfo *sura, int aya)
{
    m_sura = sura;
    m_aya = aya;

    QString subdir = "MemorizeQuran/recitations/Alafasy_128kbps";
    QString filename = QString("%1%2.mp3").arg(sura->index, 3, 10, QChar('0')).arg(aya, 3, 10, QChar('0'));

    QString downloadFolder = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    m_currentFilename = QDir::cleanPath(downloadFolder).append("/").append(subdir).append("/").append(filename);
    m_currentUrl = QString("http://www.everyayah.com/data/Alafasy_128kbps/").append(filename);

    m_player->setHasNext(aya < sura->ayas);
    m_player->setHasPrevious(aya > 1);

    m_player->setCurrentFilename(m_currentFilename);
}

void CachingPlayer::startDownload()
{
    QFileInfo fInfo(m_currentFilename);
    QDir dir = fInfo.dir();
    if (!dir.exists() && !dir.mkpath(dir.path())) {
        QMessageBox::critical(this, "Download error", QString("Could not create directory at %1").arg(dir.path()));
        return;
    }

    m_progressBar->setValue(0);
    m_mainLayout->setCurrentIndex(1);
    m_downloadManager.download(m_currentUrl, m_currentFilename);
}

void CachingPlayer::dlProgress(quint64 read, quint64 total)
{
    int percent = (int)(read * 100 / (double)total);
    m_progressBar->setValue(percent);
}

void CachingPlayer::dlError(const QString &message)
{
    QMessageBox::critical(this, "Download error", message);
    m_mainLayout->setCurrentIndex(0);
}

void CachingPlayer::dlFinished()
{
    m_mainLayout->setCurrentIndex(0);
    m_player->start();
}
