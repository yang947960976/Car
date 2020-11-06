#ifndef SETDATE_H
#define SETDATE_H

#include <QDialog>
#include "lcdclock.h"
namespace Ui {
class Setdate;
}

class Setting : public QDialog
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = 0);
    ~Setting();

private slots:
    void on_pushButton_set_time_clicked();

private:
    Ui::Setdate *ui;
    LcdClock *clock;
};

#endif // SETDATE_H
