#include "weather.h"
#include "ui_weather.h"
#include "QPixmap"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QFile>


Weather::Weather(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QPalette pal=this->palette();
    pal.setBrush(QPalette::Background,QBrush(QPixmap("/yangtianlong/qt/car/icon/background.png")));
    setPalette(pal);


    QPalette palette = ui->plainTextEdit->palette();
    palette.setBrush(QPalette::Base, QBrush(Qt::NoBrush));
    ui->plainTextEdit->setPalette(palette);

    //天气TcpSocket初始化
    weahter_http = new QTcpSocket(this);

    QObject::connect(weahter_http,SIGNAL(connected()),this,SLOT(con_ok()));
    QObject::connect(weahter_http,SIGNAL(readyRead()),this,SLOT(recv_data()));

    //添加预选城市
    QStringList city_list;
    city_list<< "广州市" << "深圳市" << "珠海市"<< "香港" << "北京市" << "上海" << "澳门";
    ui->comboBox_select_city->addItems(city_list);
    on_pushButton_query_clicked();


}

Weather::~Weather()
{
    delete ui;
}


void Weather::on_pushButton_query_clicked()
{
//    ui->pushButton_query->;


    weahter_http->abort();
    weahter_http->connectToHost(QHostAddress("101.201.182.8"),80);

    //发生一个 HTTP 请求
    if(ui->comboBox_select_city->currentIndex() == -1)
        ui->comboBox_select_city->setCurrentIndex(0);

    QString city_name = QString(ui->comboBox_select_city->currentText());
    QString msg = QString("POST /weather/query?city=%1 HTTP/1.1\r\nHost:jisutqybmf.market.alicloudapi.com\r\nAuthorization: APPCODE 6951354ec01944a09af52fd2c6f130d5\r\n\r\n").arg(city_name);

/*

AppKey：203846389
AppSecret：BQi2QkSnPCEEZ9WIPYsAAn7OiCrmts6V
AppCode：6951354ec01944a09af52fd2c6f130d5
jisutqybmf.market.alicloudapi.com
ip  101.201.182.8

*/

    int ret = weahter_http->write(msg.toUtf8());
    qDebug() << "sand"<<ret<<"byte";
//    qDebug() << msg;
}


void Weather::recv_data()
{
    //1、读到一次接受的数据
    QByteArray msg_all = weahter_http->readAll();

//    QByteArray msg_all("{\"status\":0,\"msg\":\"ok\",\"result\":{\"city\":\"北京\",\"cityid\":1,\"citycode\":\"101010100\",\"date\":\"2020-10-29\",\"week\":\"星期四\",\"weather\":\"晴\",\"temp\":\"17\",\"temphigh\":\"17\",\"templow\":\"3\",\"img\":\"0\",\"humidity\":\"30\",\"pressure\":\"1021\",\"windspeed\":\"1.8\",\"winddirect\":\"西南风\",\"windpower\":\"2级\",\"updatetime\":\"2020-10-29 16:55:00\",\"index\":[{\"iname\":\"空调指数\",\"ivalue\":\"较少开启\",\"detail\":\"您将感到很舒适，一般不需要开启空调。\"},{\"iname\":\"运动指数\",\"ivalue\":\"较不宜\",\"detail\":\"天气较好，但考虑天气寒冷，推荐您进行室内运动，户外运动时请注意保暖并做好准备活动。\"},{\"iname\":\"紫外线指数\",\"ivalue\":\"弱\",\"detail\":\"紫外线强度较弱，建议出门前涂擦SPF在12-15之间、PA+的防晒护肤品。\"},{\"iname\":\"感冒指数\",\"ivalue\":\"较易发\",\"detail\":\"天凉，昼夜温差较大，较易发生感冒，请适当增减衣服，体质较弱的朋友请注意适当防护。\"},{\"iname\":\"洗车指数\",\"ivalue\":\"较适宜\",\"detail\":\"较适宜洗车，未来一天无雨，风力较小，擦洗一新的汽车至少能保持一天。\"},{\"iname\":\"空气污染扩散指数\",\"ivalue\":\"中\",\"detail\":\"气象条件对空气污染物稀释、扩散和清除无明显影响，易感人群应适当减少室外活动时间。\"},{\"iname\":\"穿衣指数\",\"ivalue\":\"较冷\",\"detail\":\"建议着厚外套加毛衣等服装。年老体弱者宜着大衣、呢外套加羊毛衫。\"}],\"aqi\":{\"so2\":\"5\",\"so224\":\"3\",\"no2\":\"45\",\"no224\":\"51\",\"co\":\"0.450\",\"co24\":\"0.550\",\"o3\":\"53\",\"o38\":\"30\",\"o324\":\"30\",\"pm10\":\"63\",\"pm1024\":\"66\",\"pm2_5\":\"39\",\"pm2_524\":\"26\",\"iso2\":\"3\",\"ino2\":\"23\",\"ico\":\"5\",\"io3\":\"17\",\"io38\":\"16\",\"ipm10\":\"51\",\"ipm2_5\":\"48\",\"aqi\":\"51\",\"primarypollutant\":\"PM10\",\"quality\":\"良\",\"timepoint\":\"2020-10-29 16:00:00\",\"aqiinfo\":{\"level\":\"二级\",\"color\":\"#FFFF00\",\"affect\":\"空气质量可接受，但某些污染物可能对极少数异常敏感人群健康有较弱影响\",\"measure\":\"极少数异常敏感人群应减少户外活动\"}},\"daily\":[{\"date\":\"2020-10-29\",\"week\":\"星期四\",\"sunrise\":\"06:39\",\"sunset\":\"17:17\",\"night\":{\"weather\":\"晴\",\"templow\":\"3\",\"img\":\"0\",\"winddirect\":\"西南风\",\"windpower\":\"微风\"},\"day\":{\"weather\":\"多云\",\"temphigh\":\"17\",\"img\":\"1\",\"winddirect\":\"西南风\",\"windpower\":\"微风\"}},{\"date\":\"2020-10-30\",\"week\":\"星期五\",\"sunrise\":\"06:40\",\"sunset\":\"17:15\",\"night\":{\"weather\":\"阴\",\"templow\":\"7\",\"img\":\"2\",\"winddirect\":\"西南风\",\"windpower\":\"微风\"},\"day\":{\"weather\":\"多云\",\"temphigh\":\"17\",\"img\":\"1\",\"winddirect\":\"南风\",\"windpower\":\"微风\"}},{\"date\":\"2020-10-31\",\"week\":\"星期六\",\"sunrise\":\"06:41\",\"sunset\":\"17:14\",\"night\":{\"weather\":\"晴\",\"templow\":\"6\",\"img\":\"0\",\"winddirect\":\"北风\",\"windpower\":\"微风\"},\"day\":{\"weather\":\"晴\",\"temphigh\":\"19\",\"img\":\"0\",\"winddirect\":\"西北风\",\"windpower\":\"微风\"}},{\"date\":\"2020-11-01\",\"week\":\"星期日\",\"sunrise\":\"06:42\",\"sunset\":\"17:13\",\"night\":{\"weather\":\"晴\",\"templow\":\"4\",\"img\":\"0\",\"winddirect\":\"西北风\",\"windpower\":\"微风\"},\"day\":{\"weather\":\"多云\",\"temphigh\":\"17\",\"img\":\"1\",\"winddirect\":\"西北风\",\"windpower\":\"3-5级\"}},{\"date\":\"2020-11-02\",\"week\":\"星期一\",\"sunrise\":\"06:43\",\"sunset\":\"17:12\",\"night\":{\"weather\":\"晴\",\"templow\":\"2\",\"img\":\"0\",\"winddirect\":\"东北风\",\"windpower\":\"微风\"},\"day\":{\"weather\":\"晴\",\"temphigh\":\"15\",\"img\":\"0\",\"winddirect\":\"西北风\",\"windpower\":\"3-5级\"}},{\"date\":\"2020-11-03\",\"week\":\"星期二\",\"sunrise\":\"06:44\",\"sunset\":\"17:11\",\"night\":{\"weather\":\"晴\",\"templow\":\"2\",\"img\":\"0\",\"winddirect\":\"西南风\",\"windpower\":\"微风\"},\"day\":{\"weather\":\"晴\",\"temphigh\":\"15\",\"img\":\"0\",\"winddirect\":\"西南风\",\"windpower\":\"微风\"}},{\"date\":\"2020-11-04\",\"week\":\"星期三\",\"sunrise\":\"06:45\",\"sunset\":\"17:09\",\"night\":{\"weather\":\"晴\",\"templow\":\"3\",\"img\":\"0\",\"winddirect\":\"北风\",\"windpower\":\"微风\"},\"day\":{\"weather\":\"多云\",\"temphigh\":\"15\",\"img\":\"1\",\"winddirect\":\"南风\",\"windpower\":\"微风\"}}],\"hourly\":[{\"time\":\"17:00\",\"weather\":\"晴\",\"temp\":\"15\",\"img\":\"0\"},{\"time\":\"18:00\",\"weather\":\"晴\",\"temp\":\"13\",\"img\":\"0\"},{\"time\":\"19:00\",\"weather\":\"晴\",\"temp\":\"11\",\"img\":\"0\"},{\"time\":\"20:00\",\"weather\":\"晴\",\"temp\":\"9\",\"img\":\"0\"},{\"time\":\"21:00\",\"weather\":\"晴\",\"temp\":\"8\",\"img\":\"0\"},{\"time\":\"22:00\",\"weather\":\"晴\",\"temp\":\"7\",\"img\":\"0\"},{\"time\":\"23:00\",\"weather\":\"晴\",\"temp\":\"6\",\"img\":\"0\"},{\"time\":\"0:00\",\"weather\":\"晴\",\"temp\":\"6\",\"img\":\"0\"},{\"time\":\"1:00\",\"weather\":\"晴\",\"temp\":\"5\",\"img\":\"0\"},{\"time\":\"2:00\",\"weather\":\"晴\",\"temp\":\"5\",\"img\":\"0\"},{\"time\":\"3:00\",\"weather\":\"晴\",\"temp\":\"4\",\"img\":\"0\"},{\"time\":\"4:00\",\"weather\":\"晴\",\"temp\":\"4\",\"img\":\"0\"},{\"time\":\"5:00\",\"weather\":\"晴\",\"temp\":\"3\",\"img\":\"0\"},{\"time\":\"6:00\",\"weather\":\"晴\",\"temp\":\"3\",\"img\":\"0\"},{\"time\":\"7:00\",\"weather\":\"晴\",\"temp\":\"3\",\"img\":\"0\"},{\"time\":\"8:00\",\"weather\":\"晴\",\"temp\":\"7\",\"img\":\"0\"},{\"time\":\"9:00\",\"weather\":\"晴\",\"temp\":\"7\",\"img\":\"0\"},{\"time\":\"10:00\",\"weather\":\"晴\",\"temp\":\"11\",\"img\":\"0\"},{\"time\":\"11:00\",\"weather\":\"晴\",\"temp\":\"14\",\"img\":\"0\"},{\"time\":\"12:00\",\"weather\":\"晴\",\"temp\":\"15\",\"img\":\"0\"},{\"time\":\"13:00\",\"weather\":\"晴\",\"temp\":\"16\",\"img\":\"0\"},{\"time\":\"14:00\",\"weather\":\"晴\",\"temp\":\"17\",\"img\":\"0\"},{\"time\":\"15:00\",\"weather\":\"多云\",\"temp\":\"17\",\"img\":\"1\"},{\"time\":\"16:00\",\"weather\":\"多云\",\"temp\":\"17\",\"img\":\"1\"}]}}");


    //3、去头
    QByteArray sub = msg_all.mid(msg_all.indexOf('{'), msg_all.lastIndexOf('}')
                                 - msg_all.indexOf('{') + 1);

//    qDebug() <<sub;

    //4、格式化收到的json数据
    QJsonParseError err;//错误对象
    QJsonDocument  root=QJsonDocument::fromJson(sub,&err);
    if(err.error != QJsonParseError::NoError)
    {
//        qDebug()<<"encode JSON data err";
//        qDebug() <<sub;
//          on_pushButton_query_clicked();
        return;
    }

    qDebug() << "encode JSON succeed";

    QJsonObject jsonobj = root.object();
    QJsonValue  all_data = jsonobj.take("result");
    QJsonObject result= all_data.toObject();

    QJsonValue city_name = result.take("city");
    qDebug()<<city_name.toString();

    //显示今天的温度
    ui->label_today_wendu->setText(result.take("temp").toString());
    //显示今天的湿度
    ui->label_today_shidu->setText(QString(result.take("humidity").toString()+"%"));
    //显示空气指数
    QJsonObject aqi = result.take("aqi").toObject();
    QString quality = aqi.take("quality").toString()+aqi.take("aqi").toString();
    ui->label_today_quality->setText(quality);

    //友好提示
    QJsonObject aqiinfo = aqi.take("aqiinfo").toObject();
    QString affect = aqiinfo.take("affect").toString();
//    qDebug() << "affect"<<affect;
    if(affect.isEmpty())
        ui->plainTextEdit->setPlainText("没充钱，不给小提示");
    else
        ui->plainTextEdit->setPlainText(affect);

    //求出数组的大小
    QJsonArray daily_arry   =  result.take("daily").toArray();
    int daily_arry_size =  daily_arry.size();
        qDebug() << daily_arry_size;


     //遍历数组
     QJsonValue  value;
     for(int i =0;i<daily_arry_size;i++)
     {
         qDebug() << i;
         value =  daily_arry.at(i);
         if(i==0)
         {
             change_show(i,value);
         }
         else if(i==1)
         {
             change_show(i,value);
         }
         else if(i==2)
         {
             change_show(i,value);
         }
         else if(i==3)
         {
             change_show(i,value);
         }
         else if(i==4)
         {
             change_show(i,value);
         }
         else if(i==5)
         {
             change_show(i,value);
         }

     }

     //2、读完数据后断开连接
     weahter_http->disconnectFromHost();

}

void Weather::change_show(int n, QJsonValue &data)
{
    QJsonObject day = data.toObject().take("day").toObject();
    QJsonObject night = data.toObject().take("night").toObject();

    if(n==0)
    {
        //今天
        //星期
//        ui->label_today->setText(data.toObject().take("week").toString());
        //日期
        ui->label_today_ymd->setText(data.toObject().take("date").toString());
        //天气
        QString type = QString(day.take("weather").toString());
        ui->label_today_type->setText(type);
        ui->label_today_type_2->setText(type);
        //显示图片
        QString img_code = day.take("img").toString();
        show_icon(*(ui->label_today_img),img_code);
        //一天温度
        QString l = QString(night.take("templow").toString());
        QString h = QString(day.take("temphigh").toString());
        QString hl = QString("%1-%2℃").arg(l).arg(h);
        ui->label_today_low_higth_2->setText(hl);
        ui->label_today_low_higth->setText(hl);

        //风向
        QString fxfl = day.take("winddirect").toString();
        ui->label_today_fxfl_2->setText(fxfl);
        ui->label_today_fxfl->setText(fxfl);
    }
    else if(n ==1 )
    {
        //第1天
        //星期
//        ui->label_day1->setText(data.toObject().take("week").toString());
        //日期
        ui->label_day1_ymd->setText(data.toObject().take("date").toString());
        //天气
        ui->label_day1_type->setText(day.take("weather").toString());
        //显示图片
        QString img_code = day.take("img").toString();
        show_icon(*(ui->label_day1_img),img_code);
        //一天温度
        QString l = QString(night.take("templow").toString());
        QString h = QString(day.take("temphigh").toString());
        QString hl = QString("%1-%2℃").arg(l).arg(h);
        ui->label_day1_low_higth->setText(hl);

        //风向
        QString fxfl = day.take("winddirect").toString();
        ui->label_day1_fxfl->setText(fxfl);
    }
    else if(n==2)
    {
        //第1天
        //星期
        ui->label_day2->setText(data.toObject().take("week").toString());
        //日期
        ui->label_day2_ymd->setText(data.toObject().take("date").toString());
        //天气
        ui->label_day2_type->setText(day.take("weather").toString());
        //显示图片
        QString img_code = day.take("img").toString();
        show_icon(*(ui->label_day2_img),img_code);

        //一天温度
        QString l = QString(night.take("templow").toString());
        QString h = QString(day.take("temphigh").toString());
        QString hl = QString("%1-%2℃").arg(l).arg(h);
        ui->label_day2_low_higth->setText(hl);

        //风向
        QString fxfl = day.take("winddirect").toString();
        ui->label_day2_fxfl->setText(fxfl);
    }
    else if(n==3)
    {
        //第1天
        //星期
        ui->label_day3->setText(data.toObject().take("week").toString());
        //日期
        ui->label_day3_ymd->setText(data.toObject().take("date").toString());
        //天气
        ui->label_day3_type->setText(day.take("weather").toString());
        //显示图片
        QString img_code = day.take("img").toString();
        show_icon(*(ui->label_day3_img),img_code);

        //一天温度
        QString l = QString(night.take("templow").toString());
        QString h = QString(day.take("temphigh").toString());
        QString hl = QString("%1-%2℃").arg(l).arg(h);
        ui->label_day3_low_higth->setText(hl);

        //风向
        QString fxfl = day.take("winddirect").toString();
        ui->label_day3_fxfl->setText(fxfl);
    }
    else if(n==4)
    {
        //第1天
        //星期
        ui->label_day4->setText(data.toObject().take("week").toString());
        //日期
        ui->label_day4_ymd->setText(data.toObject().take("date").toString());
        //天气
        ui->label_day4_type->setText(day.take("weather").toString());
        //显示图片
        QString img_code = day.take("img").toString();
        show_icon(*(ui->label_day4_img),img_code);

        //一天温度
        QString l = QString(night.take("templow").toString());
        QString h = QString(day.take("temphigh").toString());
        QString hl = QString("%1-%2℃").arg(l).arg(h);
        ui->label_day4_low_higth->setText(hl);

        //风向
        QString fxfl = day.take("winddirect").toString();
        ui->label_day4_fxfl->setText(fxfl);
    }
    else if(n==5)
    {
        //第1天
        //星期
        ui->label_day5->setText(data.toObject().take("week").toString());
        //日期
        ui->label_day5_ymd->setText(data.toObject().take("date").toString());
        //天气
        ui->label_day5_type->setText(day.take("weather").toString());
        //显示图片
        QString img_code = day.take("img").toString();
        show_icon(*(ui->label_day5_img),img_code);

        //一天温度
        QString l = QString(night.take("templow").toString());
        QString h = QString(day.take("temphigh").toString());
        QString hl = QString("%1-%2℃").arg(l).arg(h);
        ui->label_day5_low_higth->setText(hl);

        //风向
        QString fxfl = day.take("winddirect").toString();
        ui->label_day5_fxfl->setText(fxfl);
    }


}


void Weather::show_icon(QLabel &label_img,QString code)
{
//    const QString label_style =
//        "min-width:50px;min-height:50px;max-width:60px;max-height:60px;border-radius:10px;border:2px solid blue;";
//    label_img.setStyleSheet(label_style);

   QPixmap tmp;
   QString img_path = QString("/yangtianlong/qt/car/weathercn/%1.png").arg(code);
        //将图片大小调整为抱歉的大小，并保持图片的原始长宽比例
        tmp = QPixmap(img_path).scaled(label_img.size());
        label_img.setPixmap(tmp);  

}


void Weather::con_ok()
{
//    QMessageBox::information(this, "提示", "连接成功");
    qDebug()<<"connect succeed";
}


void Weather::on_comboBox_select_city_currentIndexChanged(const QString &arg1)
{
    qDebug()<<arg1;
    on_pushButton_query_clicked();
}
