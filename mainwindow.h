#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "quraninfo.h"

class QMenu;
class QLabel;
class QSpinBox;
class MyLabel;
class CachingPlayer;
class QVBoxLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

    virtual void wheelEvent(QWheelEvent *event);
    virtual void closeEvent(QCloseEvent *event);

private slots:
    void suraMenuItemToggled(bool checked);
    void textMenuItemToggled(bool checked);
    void transMenuItemToggled(bool checked);

    void ayaSpinChanged(int aya);

    void rotateIntervalToggled(bool checked);

    void showNextAya();

    void exit();

    void next();
    void previous();

private:
    int m_mouseClickX;
    int m_mouseClickY;
    bool m_dragInProgress;

    QVBoxLayout *m_mainLayout;

    QLabel *m_suraNameLabel;
    MyLabel *m_ayaTextLabel;
    QLabel *m_transLabel;

    QMenu *m_QuranTextMenu;
    QMenu *m_suraMenu;
    QMenu *m_transMenu;
    QMenu *m_rotateMenu;

    QuranInfo m_QuranInfo;
    QuranTextInfo *m_currentText;
    SuraInfo *m_currentSura;
    QuranTextInfo *m_currentTrans;
    int m_currentAya;

    QTimer m_rotateTimer;
    int m_rotateInterval;

    QSpinBox *m_ayaSpin;

    CachingPlayer *m_player;

    void setWindowStyle(bool stayOnTop);

    void initMenu();
    void initOptionsBar(QMenu *optionsMenu);
    void populateQuranTextMenu(QMenu *menu);
    void populateSuraMenu(QMenu *menu);
    void populateTransMenu(QMenu *menu);
    void populateRotateIntervalMenu(QMenu *menu);
    void loadData();

    void setCurrentText(int index);
    void setCurrentText(QuranTextInfo *textInfo);

    void setCurrentSura(int index);
    void setCurrentSura(SuraInfo *suraInfo);

    void setCurrentTrans(int index);
    void setCurrentTrans(QuranTextInfo *transInfo);

    void setCurrentAya(int index);

    void showCurrentAya();

    void setRotateInterval(int interval);
};

#endif // MAINWINDOW_H
