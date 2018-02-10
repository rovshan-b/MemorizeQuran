#include "audioplayer.h"
#include <QtWidgets>

AudioPlayer::AudioPlayer(QWidget *parent) : QWidget(parent), m_playerState(QMediaPlayer::StoppedState), m_duration(0)
{
    m_playButton = new QToolButton(this);
    m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    m_playButton->setAutoRaise(true);
    m_playButton->setIconSize(QSize(14,14));
    m_playButton->setToolTip("Play");

    connect(m_playButton, SIGNAL(clicked()), this, SLOT(playClicked()));

    m_stopButton = new QToolButton(this);
    m_stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    m_stopButton->setEnabled(false);
    m_stopButton->setAutoRaise(true);
    m_stopButton->setIconSize(QSize(14,14));
    m_stopButton->setToolTip("Stop");

    connect(m_stopButton, SIGNAL(clicked()), this, SIGNAL(stop()));

    m_nextButton = new QToolButton(this);
    m_nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    m_nextButton->setAutoRaise(true);
    m_nextButton->setIconSize(QSize(14,14));
    m_nextButton->setToolTip("Next aya");

    connect(m_nextButton, SIGNAL(clicked()), this, SIGNAL(next()));

    m_previousButton = new QToolButton(this);
    m_previousButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    m_previousButton->setAutoRaise(true);
    m_previousButton->setIconSize(QSize(14,14));
    m_previousButton->setToolTip("Previous aya");

    connect(m_previousButton, SIGNAL(clicked()), this, SIGNAL(previous()));

    m_slider = new QSlider(Qt::Horizontal, this);
    m_slider->setRange(0, m_player.duration() / 1000);

    m_labelDuration = new QLabel(this);
    QFont f = m_labelDuration->font();
    f.setPointSize(8);
    m_labelDuration->setFont(f);
    connect(m_slider, SIGNAL(sliderMoved(int)), this, SLOT(seek(int)));

    m_autoNextButton = new QToolButton(this);
    m_autoNextButton->setIcon(style()->standardIcon(QStyle::QStyle::SP_MediaSkipForward));
    m_autoNextButton->setCheckable(true);
    m_autoNextButton->setAutoRaise(true);
    m_autoNextButton->setIconSize(QSize(14,14));
    m_autoNextButton->setToolTip("Play continuously");

    QBoxLayout *layout = new QHBoxLayout();
    layout->setMargin(0);

    layout->addWidget(m_stopButton);
    layout->addWidget(m_previousButton);
    layout->addWidget(m_playButton);
    layout->addWidget(m_nextButton);

    layout->addWidget(m_slider, 1);
    layout->addWidget(m_labelDuration);
    layout->addWidget(m_autoNextButton);

    setLayout(layout);


    connect(&m_player, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(&m_player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(&m_player, SIGNAL(metaDataChanged()), SLOT(metaDataChanged()));
    connect(&m_player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this, SLOT(statusChanged(QMediaPlayer::MediaStatus)));
    connect(&m_player, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(setState(QMediaPlayer::State)));
    connect(&m_player, SIGNAL(bufferStatusChanged(int)), this, SLOT(bufferingProgress(int)));
    connect(&m_player, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(displayErrorMessage()));

    connect(this, SIGNAL(play()), &m_player, SLOT(play()));
    connect(this, SIGNAL(pause()), &m_player, SLOT(pause()));
    connect(this, SIGNAL(stop()), &m_player, SLOT(stop()));

    setState(m_player.state());

    //m_player.setMedia(QUrl("http://www.everyayah.com/data/Alafasy_128kbps/001001.mp3"));

}

void AudioPlayer::setMedia(const QMediaContent &source)
{
    m_player.stop();
    m_player.setMedia(source);
    m_player.play();
}

void AudioPlayer::start()
{
    this->playClicked();
}

void AudioPlayer::stopPlayer()
{
    m_player.stop();
}

bool AudioPlayer::isPlaying() const
{
    return m_player.state() == QMediaPlayer::PlayingState;
}

void AudioPlayer::setState(QMediaPlayer::State state)
{
    if (state != m_playerState) {
        m_playerState = state;

        switch (state) {
        case QMediaPlayer::StoppedState:
            m_stopButton->setEnabled(false);
            m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        case QMediaPlayer::PlayingState:
            m_stopButton->setEnabled(true);
            m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            break;
        case QMediaPlayer::PausedState:
            m_stopButton->setEnabled(true);
            m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        }
    }
}

void AudioPlayer::setHasNext(bool has)
{
    m_nextButton->setEnabled(has);
}

void AudioPlayer::setHasPrevious(bool has)
{
    m_previousButton->setEnabled(has);
}

void AudioPlayer::setCurrentFilename(const QString &filename)
{
    m_currentFilename = filename;
}

void AudioPlayer::playClicked()
{
    switch (m_playerState) {
    case QMediaPlayer::StoppedState:
    {
        if (QFile::exists(m_currentFilename)) {
            setMedia(QUrl::fromLocalFile(m_currentFilename));
            emit play();
        } else {
            emit fileNotFound();
        }
        break;
    }
    case QMediaPlayer::PausedState:
        emit play();
        break;
    case QMediaPlayer::PlayingState:
        emit pause();
        break;
    }
}

void AudioPlayer::durationChanged(qint64 duration)
{
    m_duration = duration/1000;
    m_slider->setMaximum(duration / 1000);
}

void AudioPlayer::positionChanged(qint64 progress)
{
    if (!m_slider->isSliderDown()) {
        m_slider->setValue(progress / 1000);
    }
    updateDurationInfo(progress / 1000);
}

void AudioPlayer::metaDataChanged()
{
    if (m_player.isMetaDataAvailable()) {
        setTrackInfo(QString("%1 - %2")
                .arg(m_player.metaData(QMediaMetaData::AlbumArtist).toString())
                .arg(m_player.metaData(QMediaMetaData::Title).toString()));

        /*if (coverLabel) {
            QUrl url = m_player.metaData(QMediaMetaData::CoverArtUrlLarge).value<QUrl>();

            coverLabel->setPixmap(!url.isEmpty()
                    ? QPixmap(url.toString())
                    : QPixmap());
        }*/
    }
}

void AudioPlayer::seek(int seconds)
{
    m_player.setPosition(seconds * 1000);
}

void AudioPlayer::statusChanged(QMediaPlayer::MediaStatus status)
{
    handleCursor(status);

    // handle status message
    switch (status) {
    case QMediaPlayer::UnknownMediaStatus:
    case QMediaPlayer::NoMedia:
    case QMediaPlayer::LoadedMedia:
    case QMediaPlayer::BufferingMedia:
    case QMediaPlayer::BufferedMedia:
        setStatusInfo(QString());
        break;
    case QMediaPlayer::LoadingMedia:
        setStatusInfo(tr("Loading..."));
        break;
    case QMediaPlayer::StalledMedia:
        setStatusInfo(tr("Media Stalled"));
        break;
    case QMediaPlayer::EndOfMedia:
    {
        m_slider->setValue(0);
        emit endOfMedia();
        if (m_autoNextButton->isChecked()) {
            emit next();
        }
        break;
    }
    case QMediaPlayer::InvalidMedia:
        displayErrorMessage();
        break;
    }
}

void AudioPlayer::bufferingProgress(int progress)
{
    setStatusInfo(tr("Buffering %4%").arg(progress));
}

void AudioPlayer::displayErrorMessage()
{
    setStatusInfo(m_player.errorString());
}

void AudioPlayer::setTrackInfo(const QString &info)
{
    m_trackInfo = info;
   /* if (!m_statusInfo.isEmpty())
        setWindowTitle(QString("%1 | %2").arg(m_trackInfo).arg(m_statusInfo));
    else
        setWindowTitle(m_trackInfo);*/
}

void AudioPlayer::handleCursor(QMediaPlayer::MediaStatus status)
{
#ifndef QT_NO_CURSOR
    if (status == QMediaPlayer::LoadingMedia ||
        status == QMediaPlayer::BufferingMedia ||
        status == QMediaPlayer::StalledMedia)
        setCursor(QCursor(Qt::BusyCursor));
    else
        unsetCursor();
#endif
}

void AudioPlayer::setStatusInfo(const QString &info)
{
    m_statusInfo = info;
    /*QString line;
    if (!m_statusInfo.isEmpty())
        line = m_statusInfo;
    else
        line = m_trackInfo;

    setToolTip(line);*/
}

void AudioPlayer::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    if (currentInfo || m_duration) {
        QTime currentTime((currentInfo/3600)%60, (currentInfo/60)%60, currentInfo%60, (currentInfo*1000)%1000);
        QTime totalTime((m_duration/3600)%60, (m_duration/60)%60, m_duration%60, (m_duration*1000)%1000);
        QString format = "mm:ss";
        if (m_duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    m_labelDuration->setText(tStr);
}

