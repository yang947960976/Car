#include "video.h"
#include "ui_video.h"
#include <QDebug>

Video::Video(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Video)
{
    ui->setupUi(this);

    QPalette pal=this->palette();
    pal.setBrush(QPalette::Background,QBrush(QPixmap("/yangtianlong/qt/car/icon/background.png")));
    setPalette(pal);

    //关联进程的可读信号
    play = new QProcess(this);
    connect(play,SIGNAL(readyReadStandardOutput()),this,SLOT(read_all()));
    timer = new QTimer(this);
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(onTimeOut()));

    //初始化播放状态
    mid_flag = false;
    state = false;

    //初始化文件链表
    m_nFilesCount=0;
    m_nDirCount=0;
    video_list = new QMap<QString,QString>;
    FindFile("./video");
    //遍历添加到歌单中
    for(QMap<QString,QString>::iterator it=video_list->begin();it!=video_list->end();++it)
    {
        qDebug()<<it.key()<<":"<<it.value();
        QString name = it.key();
        ui->show_video_list->addItem(name.toUtf8());
    }
    ui->show_video_list->setCurrentRow(0);

    //隐藏播放列表
    ui->show_video_list->hide();
    ui->video_list_title->hide();
    list_hide = true;

}

Video::~Video()
{
    delete ui;
}

void Video::on_pushButton_mid_clicked()
{
    if(mid_flag==false)//未音乐播放
    {   //true播放

        QString img_path = QString(":/icon/play.png");
        ui->pushButton_mid->setIconSize(ui->pushButton_mid->size());
        ui->pushButton_mid->setIcon(QIcon(img_path));
        if(state == false)//未播放
        {
            start_player();
        }
        else
        {
            qDebug()<<"playing";
            timer->start(1000);
            play->write("pause\n");

        }
        state = true;
        mid_flag = true;

    }
    else
    {   //false暂停
        mid_flag = false;
        QString img_path = QString(":/icon/pause.png");
        ui->pushButton_mid->setIconSize(ui->pushButton_mid->size());
        ui->pushButton_mid->setIcon(QIcon(img_path));
        qDebug()<<"pause";
        play->write("pause\n");
        timer->stop();

    }
}

//启动播放器，并播放第一首
void Video::start_player()
{

    QString num = QString("volume %1 1\n").arg(ui->volume->value());
    QString video_path = QString("mplayer -slave -quiet -zoom -x 633 -y 380 %1")
            .arg(get_current_music_path());
    play->start(video_path);
    play->write(num.toUtf8());
    qDebug()<<video_path;
    get_video_info();
    timer->start(1000);


}

//获取视频信息
void Video::get_video_info()
{
    play->write("get_time_pos\n");
    play->write("get_time_length\n");
    play->write("get_meta_artist\n");
    play->write("get_file_name\n");
}


//读取进程返回的信息
void Video::read_all()
{
       encode();
}

//解析mplayer的回文
void Video::encode()
{
    while(play->canReadLine())
    {
        QByteArray byte(play->readLine());
        byte.replace((QByteArray("\n")),QByteArray(""));
        QString info(byte);
        //获取总时长
//        qDebug()<<"info:"<<info;
        if(byte.startsWith("ANS_LENGTH"))
        {

            int totalTimeNum = info.mid(11).toFloat()-5;
            musicTotalTime = totalTimeNum;
            int sec = 0,min = 0;
            if(totalTimeNum<60)
            {
                sec = totalTimeNum;
                min = 0;
            }else if(totalTimeNum >= 60 && totalTimeNum < 3600 )
            {
                sec = totalTimeNum % 60;
                min = totalTimeNum / 60;
            }
            QString music_time = QString("%1:%2").arg(min).arg(sec);
            ui->label_time_end->setText(music_time);
            ui->progress_bar->setMaximum(totalTimeNum);
            ui->progress_bar->setMinimum(0);
//            qDebug()<<"time:"<<music_time;
        }
        //获取当前播放时间

        if(byte.startsWith("ANS_TIME_POSITION"))
        {
//            qDebug()<<info;
            int totalTimeNum = info.mid(18).toFloat();
            speed_time =totalTimeNum;
            int sec = 0,min = 0;
            if(totalTimeNum<60)
            {
                sec = totalTimeNum;
                min = 0;
            }else if(totalTimeNum >= 60 && totalTimeNum < 3600 )
            {
                sec = totalTimeNum % 60;
                min = totalTimeNum / 60;
            }
            QString music_time = QString("%1:%2").arg(min).arg(sec);
            ui->label_time_begin->setText(music_time);
            ui->progress_bar->setValue(totalTimeNum);

            if(totalTimeNum >= musicTotalTime&&musicTotalTime>0)
                on_pushButton_next_clicked();
        }
    }
}



void Video::onTimeOut()
{

    if(state == true)
    {
        if(mid_flag == true)
        {
//            play->write("get_file_name\n");
            play->write("get_time_pos\n");
            play->write("get_time_length\n");
        }
    }
}


//遍历音频文件
bool  Video::FindFile(const QString &path)
{
    QDir dir(path);
    if(!dir.exists())
        return false;
    //筛选文件夹和文件
    dir.setFilter(QDir::Dirs|QDir::Files);
    //文件排在前面
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();
    int i=0;

    bool bIsDir;
    do{
        QFileInfo fileinfo = list.at(i);
        if((fileinfo.fileName() == "." )|(fileinfo.fileName() == ".."))
        {
            ++i;
            continue;
        }
        QString filename = fileinfo.fileName();

        bIsDir =fileinfo.isDir();
        if(bIsDir)
        {
            ++m_nDirCount;
            FindFile(fileinfo.filePath());
        }
        else if(filename.contains(".mp4",Qt::CaseSensitive)|filename.contains(".avi",Qt::CaseSensitive))
        {
            ++m_nFilesCount;
            video_list->insert(fileinfo.fileName(),fileinfo.filePath());
//            qDebug()<<fileinfo.filePath();
//            qDebug()<<fileinfo.fileName();
        }
        ++i;

    }while(i < list.size());


    return true;

}

//获取音乐列表的音乐路径
QString Video::get_current_music_path()
{
    QString name = ui->show_video_list->currentItem()->text();
    QMap<QString, QString>::const_iterator it = video_list->find(name);

    QString path = it.value();
    return path;


}


//播放另一首
void Video::player_nay(QString path)
{
    QString music_path = QString("loadfile %1\n").arg(path);
    qDebug()<<music_path;
    play->write(music_path.toUtf8());
    get_video_info();

}

void Video::on_pushButton_next_clicked()
{
    int current = ui->show_video_list->currentRow();
    if(current == ui->show_video_list->count()-1)
        current = 0;
    else
        current++;
    ui->show_video_list->setCurrentRow(current);
}

void Video::on_volume_valueChanged(int value)
{
    QString num = QString("volume %1 1\n").arg(value);
    play->write(num.toUtf8());
    qDebug()<<"volume :"<<num;
}

void Video::on_progress_bar_sliderReleased()
{
    timer->start(1000);
    QString num = QString("seek %1 2\n").arg(ui->progress_bar->value());
    play->write(num.toUtf8());
    qDebug()<<num;
    QString img_path = QString(":/icon/play.png");
    ui->pushButton_mid->setIconSize(ui->pushButton_mid->size());
    ui->pushButton_mid->setIcon(QIcon(img_path));

    mid_flag = true;
}

void Video::on_progress_bar_sliderPressed()
{
    timer->stop();
}

void Video::on_pushButton_up_clicked()
{
    int current = ui->show_video_list->currentRow();
    if(current == 0)
        current = ui->show_video_list->count()-1;
    else
        current--;
    ui->show_video_list->setCurrentRow(current);
}


void Video::on_show_video_list_currentRowChanged(int currentRow)
{
    if(state == true)
    {
        qDebug()<<"on_show_music_list_currentRowChanged";
        player_nay(get_current_music_path());
        get_video_info();
    }
}

void Video::on_pushButton_back_clicked()
{
    //发送 结束信号
    mid_flag = false;
    state = false;
    play->kill();
    this->close();
}

void Video::on_pushButton_retreat_clicked()
{
    //seek <value> [type]
    QString retreat = QString("seek -10 0\n");
    play->write(retreat.toUtf8());
}

void Video::on_pushButton_speed_clicked()
{
    //seek <value> [type]
    if(speed_time<=musicTotalTime-15)
    {
        QString speed = QString("seek +10 0\n");
        play->write(speed.toUtf8());

    }

}

void Video::on_pushButton_next_2_clicked()
{
    if(list_hide)
    {
        ui->show_video_list->show();
        ui->video_list_title->show();
        list_hide = false;
    }else
    {
        ui->show_video_list->hide();
        ui->video_list_title->hide();
        list_hide = true;
    }
}
