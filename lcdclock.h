#ifndef LCDCLOCK_H
#define LCDCLOCK_H
#include <QWidget>
#include <QLCDNumber>
#include <QTimer> // 定时器

class LcdClock : public QLCDNumber
{
    // 使得当前的类支持信号与槽
    Q_OBJECT
public:
    LcdClock(QWidget *parent = 0);
    ~LcdClock();


private slots:
    void onTimeOut();

private:
    QTimer timer;
    int hour; // 分
    int minute; // 分
    int second; // 秒

};

#endif // LCDCLOCK_H
