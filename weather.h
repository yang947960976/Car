#ifndef WIDGET_H
#define WIDGET_H

#include <QDialog>
#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QLabel>
//namespace Ui {
//class Widget;
//}

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Weather : public QDialog
{
    Q_OBJECT

public:
    explicit Weather(QDialog *parent = 0);
//    void add_json_data();
    void change_show(int n,QJsonValue &data);
    ~Weather();

private slots:
    void on_pushButton_query_clicked();
    void recv_data();
    void con_ok();
    void show_icon(QLabel &label_img,QString code);

    void on_comboBox_select_city_currentIndexChanged(const QString &arg1);

private:
    Ui::Widget *ui;
    QTcpSocket *weahter_http;
    QMap<QString,QString> *city_name_code;

};

#endif // WIDGET_H
