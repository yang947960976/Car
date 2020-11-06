#ifndef MUSIC_H
#define MUSIC_H
#include <QDialog>
#include <QWidget>
#include <QProcess>
#include <QFile>
#include <QTimer>
#include <QList>
#include <QMap>
#include <QDir>
#include <QFileInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class Music; }
QT_END_NAMESPACE

class Music : public QDialog
{
    Q_OBJECT

public:
    Music(QDialog *parent = nullptr);
    void show_paly_gif();
    ~Music();

private slots:
    void on_pushButton_mid_clicked();
    void onTimeOut();
    void list_onTimeOut();
    void read_all();
    void encode();
    void on_pushButton_back_clicked();
    void on_pushButton_list_clicked();
    void on_volume_valueChanged(int value);
    void on_progress_bar_sliderReleased();
    void on_progress_bar_sliderPressed();
    void on_pushButton_next_clicked();
    void on_show_music_list_currentRowChanged(int currentRow);
    void start_player();
    void player_nay(QString path);
    void get_music_info();
    void on_pushButton_up_clicked();
    void get_text(QString path);
    void music_end();
    bool  FindFile(const QString &path);
    QString get_current_music_path();

    void on_pushButton_lrc_clicked();

//    void on_show_music_list_activated(const QModelIndex &index);

private:
    Ui::Music *ui;
    QProcess *play;
    QTimer *timer;
    QTimer *list_time;
    QMap<QString,QString> *music_list;//歌单列表
    QMap<int,QString> *text_list;//歌单列表
    bool list_time_flag;
    bool mid_flag;//true播放，false暂停
    bool state;//true已在播放，false为在播放
    bool list_hide;//播放放列表隐藏与显示，true隐藏，false显示
    bool lrc_hide;//歌词隐藏与显示，true隐藏，false显示
    int m_nFilesCount;
    int m_nDirCount;
    QFile *lrc;
    int musicTotalTime;

};
#endif // MUSIC_H
