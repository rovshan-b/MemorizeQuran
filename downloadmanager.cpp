#include "downloadmanager.h"

DownloadManager::DownloadManager(QNetworkAccessManager *manager) : m_reply(0)
{
    this->m_manager = manager;
}

void DownloadManager::download(const QUrl &url, const QString &saveFilename)
{
    if (m_reply) {
        return;
    }

    m_filename = saveFilename;

    QNetworkRequest request(url);
    //request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    m_reply = m_manager->get(request);

    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(error(QNetworkReply::NetworkError)));
    connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)),
                this, SLOT(updateProgress(qint64, qint64)));
    connect(m_reply, SIGNAL(finished()),
                this, SLOT(finished()));

    connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)),
            SLOT(sslErrors(QList<QSslError>)));
}

void DownloadManager::error(QNetworkReply::NetworkError err)
{
    Q_UNUSED(err);

    // Manage error here.
    emit onError(m_reply->errorString());

    m_reply->deleteLater();
    m_reply = 0;
}

void DownloadManager::updateProgress(qint64 read, qint64 total)
{
    emit onProgress(read, total);
}

void DownloadManager::finished()
{
    QByteArray b = m_reply->readAll();
    QFile file(m_filename);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << b;
    m_reply->deleteLater();
    m_reply = 0;

    emit onFinished();
}

void DownloadManager::sslErrors(const QList<QSslError> &sslErrors)
{
    //emit onError("SSL error");
    Q_UNUSED(sslErrors);

    m_reply->ignoreSslErrors();
}
