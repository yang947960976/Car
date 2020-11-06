#ifndef CAR_H
#define CAR_H
#include <QDialog>
#include <QWidget>
#include "lcdclock.h"
#include "ditu.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Car; }
QT_END_NAMESPACE

class Car : public QWidget
{
    Q_OBJECT

public:
    Car(QWidget *parent = nullptr);
    ~Car();

private slots:
    void on_pushButton_music_clicked();

    void on_pushButton_weather_clicked();

    void on_pushButton_video_clicked();

    void on_pushButton_set_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Car *ui;
    LcdClock *clock;
    DiTu *ditu;
    bool ditu_hide;
};
#endif // CAR_H
