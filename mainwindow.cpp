#include "mainwindow.h"
#include <QtGui>
#include <QDebug>
#include "mylabel.h"
#include "qurantextinfo.h"
#include "surainfo.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_dragInProgress(false),
    m_currentSura(0),
    m_currentTrans(0),
    m_currentAya(1),
    m_rotateInterval(0)
{
    setWindowTitle("Memorize Quran");
    setStyleSheet("QMainWindow { background-color: white }");

    QWidget *mainWidget = new QWidget(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainWidget->setLayout(mainLayout);

    m_suraNameLabel = new QLabel(this);
    m_suraNameLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_suraNameLabel);

    m_ayaTextLabel = new MyLabel(this);
    m_ayaTextLabel->setText("empty");
    m_ayaTextLabel->setWordWrap(true);
    m_ayaTextLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_ayaTextLabel, 1);

    m_transLabel = new QLabel(this);
    m_transLabel->setWordWrap(true);
    m_transLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_transLabel);

    setCentralWidget(mainWidget);

    move(200, 200);
    resize(300, 200);

    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());

    setWindowStyle(settings.value("stayOnTop", true).toBool());

    float opacity = qMin(qMax(settings.value("opacity", "1.0").toDouble(), 0.3), 1.0);
    setWindowOpacity(opacity);

    QFontDatabase::addApplicationFont(":/fonts/me_quran.ttf");
    QFont font = QFont("me_quran", 18, 1);
    font.setStyleStrategy(QFont::PreferAntialias);
    m_ayaTextLabel->setFont(font);

    loadData();
    initMenu();

    int text = qMin(qMax(settings.value("text", "1").toInt(), 1), m_QuranInfo.texts().count());
    setCurrentText(text);

    int trans = qMin(qMax(settings.value("trans", "1").toInt(), 1), 114);
    setCurrentTrans(trans);

    int sura = qMin(qMax(settings.value("sura", "1").toInt(), 1), 114);
    setCurrentSura(sura);

    int aya = settings.value("aya", "1").toInt();
    setCurrentAya(aya);

    connect(&m_rotateTimer, SIGNAL(timeout()), this, SLOT(showNextAya()));

    int rotateInterval = qMin(qMax(settings.value("rotateInterval", "0").toInt(), 0), 60);
    setRotateInterval(rotateInterval);
}

MainWindow::~MainWindow()
{
    m_currentSura = 0;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    m_mouseClickX = event->x();
    m_mouseClickY = event->y();

    m_dragInProgress = true;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    m_dragInProgress = false;
}


void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(m_dragInProgress) {
        move(event->globalX()-m_mouseClickX,event->globalY()-m_mouseClickY);
    }
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    float opacity = windowOpacity();
    int delta = event->delta();

    if (delta > 0 && opacity < 1) {
        setWindowOpacity(opacity + 0.1);
    } else if (delta < 0 && opacity > 0.3) {
        setWindowOpacity(opacity - 0.1);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);

    exit();
}

void MainWindow::initMenu()
{
    QMenu *mainMenu = new QMenu("&Options", this);

    m_QuranTextMenu = new QMenu("&Text", this);
    populateQuranTextMenu(m_QuranTextMenu);
    mainMenu->addMenu(m_QuranTextMenu);

    m_suraMenu = new QMenu("&Sura", this);
    populateSuraMenu(m_suraMenu);
    mainMenu->addMenu(m_suraMenu);

    m_transMenu = new QMenu("T&ranslation", this);
    populateTransMenu(m_transMenu);
    mainMenu->addMenu(m_transMenu);

    m_rotateMenu = new QMenu("&Rotate interval", this);
    populateRotateIntervalMenu(m_rotateMenu);
    mainMenu->addMenu(m_rotateMenu);

    mainMenu->addAction("E&xit", this, SLOT(exit()));

    QPushButton *optionsButton = new QPushButton("Options", this);
    optionsButton->setFlat(true);
    optionsButton->setMenu(mainMenu);

    ayaSpin = new QSpinBox(this);
    ayaSpin->setToolTip("Aya to display");
    connect(ayaSpin, SIGNAL(valueChanged(int)), this, SLOT(ayaSpinChanged(int)));

    QFont sf = statusBar()->font();
    sf.setPointSize(8);
    ayaSpin->setFont(sf);
    optionsButton->setFont(sf);
    statusBar()->addPermanentWidget(ayaSpin);
    statusBar()->addPermanentWidget(optionsButton);
}

void MainWindow::populateSuraMenu(QMenu *menu)
{
    QActionGroup *group = new QActionGroup(this);
    group->setExclusive(true);

    foreach (SuraInfo *suraInfo, m_QuranInfo.suras()) {
        QAction *action = new QAction(QString("%1 - %2").arg(suraInfo->index).arg(suraInfo->tname), this);
        action->setData(QVariant::fromValue(suraInfo));
        action->setCheckable(true);
        connect(action, SIGNAL(toggled(bool)), this, SLOT(suraMenuItemToggled(bool)));
        group->addAction(action);
        menu->addAction(action);
    }
}

void MainWindow::populateTransMenu(QMenu *menu)
{
    QActionGroup *group = new QActionGroup(this);
    group->setExclusive(true);

    foreach (QuranTextInfo *transInfo, m_QuranInfo.translations()) {
        QAction *action = new QAction(transInfo->title, this);
        action->setData(QVariant::fromValue(transInfo));
        action->setCheckable(true);
        connect(action, SIGNAL(toggled(bool)), this, SLOT(transMenuItemToggled(bool)));
        group->addAction(action);
        menu->addAction(action);
    }
}

void MainWindow::populateQuranTextMenu(QMenu *menu)
{
    QActionGroup *group = new QActionGroup(this);
    group->setExclusive(true);

    foreach (QuranTextInfo *textInfo, m_QuranInfo.texts()) {
        QAction *action = new QAction(textInfo->title, this);
        action->setData(QVariant::fromValue(textInfo));
        action->setCheckable(true);
        connect(action, SIGNAL(toggled(bool)), this, SLOT(textMenuItemToggled(bool)));
        group->addAction(action);
        menu->addAction(action);
    }
}

void MainWindow::populateRotateIntervalMenu(QMenu *menu)
{
    QActionGroup *group = new QActionGroup(this);
    group->setExclusive(true);

    for (int i=0; i<=60; ++i) {
        QString actionTitle = (i == 0) ? "Manual" : QString("%1 min").arg(i);
        QAction *action = new QAction(actionTitle, this);
        action->setData(i);
        action->setCheckable(true);
        connect(action, SIGNAL(toggled(bool)), this, SLOT(rotateIntervalToggled(bool)));
        group->addAction(action);
        menu->addAction(action);
    }
}

void MainWindow::loadData()
{

}

void MainWindow::setCurrentSura(int index)
{
    setCurrentSura(m_QuranInfo.sura(index));
}

void MainWindow::setCurrentSura(SuraInfo *suraInfo)
{
    if (suraInfo == m_currentSura) {
        return;
    }

    m_currentSura = suraInfo;

    QAction *action = m_suraMenu->actions().at(suraInfo->index - 1);
    if (!action->isChecked()) {
        action->setChecked(true);
    }

    ayaSpin->setRange(1, m_currentSura->ayas);
    ayaSpin->setValue(1);

    setCurrentAya(1);
    showCurrentAya();
}

void MainWindow::setCurrentTrans(int index)
{
    setCurrentTrans(m_QuranInfo.translation(index));
}

void MainWindow::setCurrentTrans(QuranTextInfo *transInfo)
{
    if (transInfo == m_currentTrans) {
        return;
    }

    m_currentTrans = transInfo;

    m_QuranInfo.loadTranslations(transInfo);

    QAction *action = m_transMenu->actions().at(transInfo->index - 1);
    if (!action->isChecked()) {
        action->setChecked(true);
    }

    showCurrentAya();
}

void MainWindow::setCurrentAya(int index)
{
    if (index == m_currentAya) {
        return;
    }

    if (!m_currentSura) {
        return;
    }

    int aya = qMin(qMax(index, 1), m_currentSura->ayas);

    m_currentAya = aya;
    ayaSpin->setValue(aya);
}

void MainWindow::setCurrentText(int index)
{
    setCurrentText(m_QuranInfo.text(index));
}

void MainWindow::setCurrentText(QuranTextInfo *textInfo)
{
    if (textInfo == m_currentText) {
        return;
    }

    m_currentText = textInfo;

    m_QuranInfo.loadAyas(textInfo);

    QAction *action = m_QuranTextMenu->actions().at(textInfo->index - 1);
    if (!action->isChecked()) {
        action->setChecked(true);
    }

    showCurrentAya();
}

void MainWindow::suraMenuItemToggled(bool checked)
{
    if (!checked) {
        return;
    }

    QAction* action = qobject_cast<QAction*>(sender());
    if (!action) {
        return;
    }
    SuraInfo * suraInfo = action->data().value<SuraInfo *>();
    if (!suraInfo) {
        return;
    }

    setCurrentSura(suraInfo);
}

void MainWindow::textMenuItemToggled(bool checked)
{
    if (!checked) {
        return;
    }

    QAction* action = qobject_cast<QAction*>(sender());
    if (!action) {
        return;
    }
    QuranTextInfo * textInfo = action->data().value<QuranTextInfo *>();
    if (!textInfo) {
        return;
    }

    setCurrentText(textInfo);
}

void MainWindow::transMenuItemToggled(bool checked)
{
    if (!checked) {
        return;
    }

    QAction* action = qobject_cast<QAction*>(sender());
    if (!action) {
        return;
    }
    QuranTextInfo * transInfo = action->data().value<QuranTextInfo *>();
    if (!transInfo) {
        return;
    }

    setCurrentTrans(transInfo);
}

void MainWindow::ayaSpinChanged(int aya)
{
    setCurrentAya(aya);
    showCurrentAya();
}

void MainWindow::setWindowStyle(bool stayOnTop)
{
    Qt::WindowFlags flags(Qt::Tool | Qt::FramelessWindowHint);
    if (stayOnTop) {
        flags |= Qt::WindowStaysOnTopHint;
    }
    setWindowFlags(flags);
}

void MainWindow::showCurrentAya()
{
    if (!m_currentSura) {
        return;
    }

    QString ayaText = m_currentSura->ayaText(m_currentAya);
    m_ayaTextLabel->setText(ayaText);
    m_ayaTextLabel->recalcFontSize();

    QString transText = m_currentSura->translationText(m_currentAya);
    m_transLabel->setText(transText);
    m_transLabel->setVisible(transText.length() > 0);

    m_suraNameLabel->setText(QString("%1. %2 (%3)").
                             arg(m_currentSura->index).
                             arg(m_currentSura->tname).
                             arg(m_currentSura->ename));
}

void MainWindow::rotateIntervalToggled(bool checked)
{
    if (!checked) {
        return;
    }

    QAction* action = qobject_cast<QAction*>(sender());
    if (!action) {
        return;
    }
    int interval = action->data().value<int>();

    setRotateInterval(interval);
}

void MainWindow::showNextAya()
{
    int nextAya = m_currentAya + 1;
    if (nextAya > m_currentSura->ayas) {
        nextAya = 1;
    }

    setCurrentAya(nextAya);
    showCurrentAya();
}

void MainWindow::exit()
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("stayOnTop", windowFlags().testFlag(Qt::WindowStaysOnTopHint));
    settings.setValue("opacity", windowOpacity());
    settings.setValue("text", m_currentText->index);
    settings.setValue("sura", m_currentSura->index);
    settings.setValue("aya", m_currentAya);
    settings.setValue("trans", m_currentTrans->index);
    settings.setValue("rotateInterval", m_rotateInterval);

    QApplication::exit(0);
}

void MainWindow::setRotateInterval(int interval)
{
    m_rotateInterval = interval;

    m_rotateTimer.stop();

    if (interval > 0) {
        m_rotateTimer.setInterval(interval * 1000 * 60);
        m_rotateTimer.start();
    }

    foreach (QAction *action, m_rotateMenu->actions()) {
        if (action->data().value<int>() == interval && !action->isChecked()) {
            action->setChecked(true);
            break;
        }
    }
}
