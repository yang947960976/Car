#ifndef DITU_H
#define DITU_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QStringList>

namespace Ui {
class DiTu;
}

class DiTu : public QWidget
{
    Q_OBJECT

public:
    explicit DiTu(QWidget *parent = 0);
    ~DiTu();
private slots:
    void con_ok();
    void recv_data();
    void inquire(QString road,QString city);
    void onTimeOut();
    void road_init();

    void on_city_currentIndexChanged(const QString &arg1);

    void on_road_activated(const QString &arg1);

private:
    Ui::DiTu *ui;
    QTcpSocket *ditu_http;
    QTimer timer;
    QStringList GZ;
    QStringList SZ;
    QStringList BJ;
};

#endif // DITU_H
