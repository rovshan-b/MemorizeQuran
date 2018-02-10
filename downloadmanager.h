#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QtCore>
#include <QtNetwork>

class DownloadManager: public QObject
{
    Q_OBJECT
    QNetworkAccessManager *m_manager;

public:
    DownloadManager(QNetworkAccessManager *manager);
    void download(const QUrl &url, const QString &saveFilename);
signals:
    void onProgress(quint64 read, quint64 total);
    void onError(const QString &message);
    void onFinished();

private slots:
    void sslErrors(const QList<QSslError> &sslErrors);
    void error(QNetworkReply::NetworkError err);
    void updateProgress(qint64 read, qint64 total);
    void finished();

private:
    QNetworkReply *m_reply;
    QString m_filename;
};

#endif // DOWNLOADMANAGER_H
