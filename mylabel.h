#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = 0);

    void recalcFontSize();

protected:
    virtual void resizeEvent(QResizeEvent * event);

private:
    int m_minFontSize;
};

#endif // MYLABEL_H
