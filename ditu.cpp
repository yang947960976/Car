#include "ditu.h"
#include "ui_ditu.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>


DiTu::DiTu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiTu)
{
    ui->setupUi(this);

    QPalette palette = ui->description->palette();
    palette.setBrush(QPalette::Base, QBrush(Qt::NoBrush));
    ui->description->setPalette(palette);
    //天气TcpSocket初始化
    ditu_http = new QTcpSocket(this);

    //connect(&timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    //timer.start(5000); // 每隔20秒触发依次超时信号
    //道路列表初始化
    road_init();
    ui->road->addItems(GZ);
//    inquire();

}

void DiTu::onTimeOut()
{
//    inquire();
}

void DiTu::road_init()
{
    // 准备好不同省份的城市列表
    GZ.append("彩频路");
    GZ.append("兴南大道");
    GZ.append("花城大道隧道");
    GZ.append("华南快速");

    SZ.append("月亮湾大道");
    SZ.append("滨河大道");
    SZ.append("西环路");

    BJ.append("西长安街");
    BJ.append("西二环");
    BJ.append("京通快速路");
}


DiTu::~DiTu()
{
    delete ui;
}

void DiTu::inquire(QString road,QString city)
{
    //    ui->pushButton_query->;

    QObject::connect(ditu_http,SIGNAL(connected()),this,SLOT(con_ok()));
    QObject::connect(ditu_http,SIGNAL(readyRead()),this,SLOT(recv_data()));

    ditu_http->connectToHost(QHostAddress("14.215.179.108"),80);

    QString msg = QString("GET /traffic/v1/road?road_name=%1&city=%2&ak=2LkS58xbm4B1WKcKcey8iqB88YiZi28t HTTP/1.1\r\nHost:api.map.baidu.com\r\n\r\n").arg(road).arg(city);

    int ret = ditu_http->write(msg.toUtf8());
    qDebug() << "sand"<<ret<<"byte";

}


void DiTu::recv_data()
{
     //1、读到一次接受的数据
     QByteArray msg_all = ditu_http->readAll();
//     qDebug() <<msg_all;

     //3、去头
     QByteArray sub = msg_all.mid(msg_all.indexOf('{'), msg_all.lastIndexOf('}')
                                  - msg_all.indexOf('{') + 1);

 //    qDebug() <<sub;

     //4、格式化收到的json数据
     QJsonParseError err;//错误对象
     QJsonDocument  jsonobj=QJsonDocument::fromJson(sub,&err);
     if(err.error != QJsonParseError::NoError)
     {

         return;
     }
     QJsonObject root = jsonobj.object();
     QJsonValue  description = root.take("description");
     ui->description->clear();
     ui->description->append(description.toString());

     QJsonArray road_traffic= root.take("road_traffic").toArray();

     int road_traffic_size =  road_traffic.size();

     //遍历数组
      for(int i =0;i<road_traffic_size;i++)
      {
         QJsonObject tem  = road_traffic[i].toObject();
         QJsonValue  road_name = tem.take("road_name");
         qDebug()<<road_name.toString();
         ui->current_posiyion->setText(road_name.toString());
/*
         QJsonArray  congestion_sections = tem.take("congestion_sections").toArray();
         int congestion_sections_size =  congestion_sections.size();
         qDebug()<<"congestion_sections_size "<<congestion_sections_size;
         //遍历数组
          for(int j =0;i<congestion_sections_size;i++)
          {
              QJsonObject tem2  = congestion_sections[j].toObject();
              QJsonValue  congestion_distance = tem2.take("congestion_distance").toDouble();
              QJsonValue  speed = tem2.take("speed").toDouble();
              QJsonValue  status = tem2.take("status").toDouble();
              QJsonValue  congestion_trend = tem2.take("congestion_trend").toString();
              QJsonValue  section_desc = tem2.take("section_desc").toString();
              qDebug()<<"congestion_distance"<<congestion_distance;
              qDebug()<<"speed "<<speed;
              qDebug()<<"status "<<status;
              qDebug()<<"congestion_trend "<<congestion_trend;
              qDebug()<<"section_desc "<<section_desc;
          }
          */
      }
}


void DiTu::con_ok()
{
//    QMessageBox::information(this, "提示", "连接成功");
    qDebug()<<"connect succeed";
}


void DiTu::on_city_currentIndexChanged(const QString &arg1)
{
    ui->road->clear();
//ui->city->currentText()
    if(arg1 == "广州市")
        ui->road->addItems(GZ);
    if(arg1 == "深圳市")
        ui->road->addItems(SZ);
    if(arg1 == "北京市")
        ui->road->addItems(BJ);
}

void DiTu::on_road_activated(const QString &arg1)
{
    inquire(arg1,ui->city->currentText());
}
