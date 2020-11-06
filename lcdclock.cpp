#include "lcdclock.h"
#include <QTime>

LcdClock::LcdClock(QWidget *parent)
    :QLCDNumber(parent),
     hour(0),
     minute(0),
     second(0)
{
    // 取得当前的调色板信息
    QPalette pal = this->palette();

    this->setDigitCount(8);
    // 将背景色设置为淡灰色
    pal.setColor(QPalette::Background, Qt::lightGray);
    this->setPalette(pal);

    resize(300, 80);

    connect(&timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    timer.start(1000); // 每隔1秒触发依次超时信号


    onTimeOut();
}


LcdClock::~LcdClock()
{

}


void LcdClock::onTimeOut()
{
    QTime now = QTime::currentTime();

    QString t = now.toString("hh:mm:ss");

    static bool colon = true;
    if(colon)
    {
        t[2] = ':';
        t[5] = ':';
        colon = false;
    }
    else
    {
        t[2] = ' ';
        t[5] = ' ';
        colon = true;
    }

    display(t);
}
