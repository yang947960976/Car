#include "setting.h"
#include "ui_setting.h"
#include <QInputDialog>
#include <QDebug>
#include <QTime>


Setting::Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setdate)
{
    ui->setupUi(this);
    clock = new LcdClock(this);
    ui->Layout_LcdClock->addWidget(clock);
    clock->show();
}

Setting::~Setting()
{
    delete ui;
}

void Setting::on_pushButton_set_time_clicked()
{
    QDateTime now = QDateTime::currentDateTime();
qDebug()<<now.toString();
    QString t = now.toString("yyyy-MM-dd hh:mm:ss");

    QString y = t.mid(0,4);
    QString M = t.mid(5,2);
    QString d = t.mid(8,2);
    QString h = t.mid(11,2);
    QString m = t.mid(14,2);

//    qDebug()<<y<<"-"<<M<<"-"<<d<<" "<<h<<":"<<m;

    int newhour =0;
    int newmin = 0;
    bool ok1,ok2;

    newhour = QInputDialog::getInt(this, "输入时","时", h.toInt(), 0, 23, 1, &ok1);
    newmin = QInputDialog::getInt(this, "输入分", "分", m.toInt(),0, 59, 1, &ok2);

    if(newhour<10)
    {
        h = QString("0%1").arg(newhour);
    }else
        h = QString("%1").arg(newhour);
    if(newmin<10)
    {
        m = QString("0%1").arg(newmin);
    }else
        m = QString("%1").arg(newmin);

    if(ok1&&ok2)
    {
        //date 062920502008.10
        QString newTime = QString("date %1%2%3%4%5.%6")
                .arg(M).arg(d).arg(h).arg(m).arg(y).arg(QString("00"));
        system(newTime.toStdString().c_str());
        //强制写入到CMOS
        qDebug()<<newTime;
        system("hwclock -w");
    }


}
