#include "mylabel.h"
#include <QtGui>

MyLabel::MyLabel(QWidget *parent) : QLabel(parent), m_minFontSize(12)
{
    //QFont f = font();
    //f.setFamily("Arial");
    //setFont(f);
    //setMargin(3);
    setAlignment(Qt::AlignCenter);
}

void MyLabel::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);

    recalcFontSize();
}

void MyLabel::recalcFontSize()
{
    /*QFont aFont=font();
    // get widget's height
    int wd_hgt = size().height();
    // get widget's width
    int wd_wdt = size().width();
    int fontPointSize=0;
    aFont.setPointSize(1);

    QString widthTestText = text();
    widthTestText.append("www");
    // calculating loop
    for (int i = 1; i < 200; i++)
    {
        aFont.setPointSize(i);
        // create FontMetrics for resized font
        QFontMetrics fm(aFont);
        // get text height for current font size
        int y = fm.height();
        // get text width for current font size
        int x = fm.width(widthTestText);
        // check if text fits widget
        if (y > wd_hgt || x > wd_wdt)
        {
            // saving maximum possible size of font
            fontPointSize = i-1;
            break;
        }
    }
    if(fontPointSize>0){
        aFont.setPointSize(qMax(fontPointSize, m_minFontSize));
        setFont(aFont);
    }*/
    QFont aFont=font();
    //aFont.setPointSize(1);
    int fontPointSize=1;
    for (int i = 1; i < 200; i++)
    {
        aFont.setPointSize(i);
        QFontMetrics fm(aFont);
        QRect boundingRect=fm.boundingRect(this->rect(),
                                           Qt::AlignHCenter | Qt::AlignVCenter | Qt::TextWordWrap,
                                           text());

        if(boundingRect.width()>this->rect().width() || boundingRect.height()>this->rect().height()){
            break;
        }else{
            fontPointSize=i;
        }


    }
    if(fontPointSize>0){
        aFont.setPointSize(qMax(fontPointSize, m_minFontSize));
        setFont(aFont);
    }
}
