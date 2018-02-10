#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include <QWidget>
#include <QtMultimedia>

class QToolButton;
class QSlider;
class QLabel;

class AudioPlayer : public QWidget
{
    Q_OBJECT
public:
    explicit AudioPlayer(QWidget *parent = 0);

    void setMedia(const QMediaContent &source);

    void start();

signals:
    void play();
    void pause();
    void stop();
    void next();
    void previous();
    void fileNotFound();

public slots:
    void setState(QMediaPlayer::State state);

    void setHasNext(bool has);
    void setHasPrevious(bool has);
    void setCurrentFilename(const QString &filename);

private slots:
    void playClicked();

    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void metaDataChanged();
    void seek(int seconds);

    void statusChanged(QMediaPlayer::MediaStatus status);
    void bufferingProgress(int progress);

    void displayErrorMessage();

private:
    void setTrackInfo(const QString &info);
    void handleCursor(QMediaPlayer::MediaStatus status);
    void setStatusInfo(const QString &info);
    void updateDurationInfo(qint64 currentInfo);

    QMediaPlayer m_player;

    QMediaPlayer::State m_playerState;

    QToolButton *m_playButton;
    QToolButton *m_stopButton;
    QToolButton *m_nextButton;
    QToolButton *m_previousButton;

    QSlider *m_slider;
    QLabel *m_labelDuration;

    qint64 m_duration;

    QString m_trackInfo;
    QString m_statusInfo;

    QString m_currentFilename;
};

#endif // AUDIOPLAYER_H
