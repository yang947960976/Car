#ifndef VIDEO_H
#define VIDEO_H

#include <QDialog>
#include <QWidget>
#include <QProcess>
#include <QFile>
#include <QTimer>
#include <QList>
#include <QMap>
#include <QDir>
#include <QFileInfo>


namespace Ui {
class Video;
}

class Video : public QDialog
{
    Q_OBJECT

public:
    explicit Video(QWidget *parent = nullptr);
    ~Video();

private slots:
    void on_pushButton_mid_clicked();
    void start_player();
    void get_video_info();
    void read_all();
    void onTimeOut();
    void encode();
    QString get_current_music_path();
    void player_nay(QString path);
    bool FindFile(const QString &path);

    void on_pushButton_next_clicked();

    void on_volume_valueChanged(int value);

    void on_progress_bar_sliderReleased();

    void on_progress_bar_sliderPressed();

    void on_pushButton_up_clicked();

    void on_show_video_list_currentRowChanged(int currentRow);

    void on_pushButton_back_clicked();

    void on_pushButton_retreat_clicked();

    void on_pushButton_speed_clicked();

    void on_pushButton_next_2_clicked();

private:
    Ui::Video *ui;
    QProcess *play;
    QTimer *timer;
    QMap<QString,QString> *video_list;//歌单列表

    bool mid_flag;//true播放，false暂停
    bool state;//true已在播放，false为在播放
    bool list_hide;
    int m_nFilesCount;
    int m_nDirCount;

    int musicTotalTime;
    int speed_time;

};

#endif // VIDEO_H
