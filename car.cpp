#include "car.h"
#include "ui_car.h"
#include "weather.h"
#include "music.h"
#include "video.h"
#include "setting.h"

#include <QDebug>



Car::Car(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Car)
{
    ui->setupUi(this);

    QPalette pal=this->palette();
    pal.setBrush(QPalette::Background,QBrush(QPixmap("/yangtianlong/qt/car/icon/background.png")));
    setPalette(pal);

    clock = new LcdClock(this);
    ui->Layout_LcdClock->addWidget(clock);
    clock->show();



    ditu = new DiTu(this);
    ui->Layout_DiTu->addWidget(ditu);

    ditu->hide();
    ditu_hide = true;


}

Car::~Car()
{
    delete ui;
}



void Car::on_pushButton_music_clicked()
{
    Music music;
    music.show();
    music.exec();
    qDebug()<<"music";
}

void Car::on_pushButton_weather_clicked()
{
    Weather weather;
    weather.show();
    weather.exec();
    qDebug()<<"weather";
}

void Car::on_pushButton_video_clicked()
{
    clock->hide();
    Video video;
    video.show();
    video.exec();
    clock->show();
    qDebug()<<"video";
}

void Car::on_pushButton_set_clicked()
{
    Setting setting;
    setting.show();
    setting.exec();
    qDebug()<<"setting";
}

void Car::on_pushButton_2_clicked()
{
    if(ditu_hide)
    {
        ditu->show();
        ditu_hide = false;
    }else
    {
        ditu->hide();
        ditu_hide = true;
    }
}
