#include "music.h"
#include "ui_music.h"
#include <QPixmap>
#include <QMovie>
#include <QSize>
#include <QDebug>
#include <string.h>
#include <string>
#include <QRegExp>
#include <QListWidgetItem>

Music::Music(QDialog *parent)
    : QDialog(parent)
    , ui(new Ui::Music)
{
    ui->setupUi(this);

    QPalette pal=this->palette();
    pal.setBrush(QPalette::Background,QBrush(QPixmap("/yangtianlong/qt/car/icon/background.png")));
    setPalette(pal);

    QPalette palette = ui->listWidget_lrc->palette();
    palette.setBrush(QPalette::Base, QBrush(Qt::NoBrush));
    ui->listWidget_lrc->setPalette(palette);

    ui->show_music_list->setPalette(palette);

    //关联进程的可读信号
    play = new QProcess(this);
    connect(play,SIGNAL(readyReadStandardOutput()),this,SLOT(read_all()));
    timer = new QTimer(this);
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(onTimeOut()));

    list_time = new QTimer(this);
    QObject::connect(list_time,SIGNAL(timeout()),this,SLOT(list_onTimeOut()));
    //初始化播放状态
    mid_flag = false;
    state = false;

    //初始化文件链表
    m_nFilesCount=0;
    m_nDirCount=0;
    music_list = new QMap<QString,QString>;
    FindFile("./music");
    //初始化歌词
    text_list= new QMap<int,QString>;

    //遍历添加到歌单中
    for(QMap<QString,QString>::iterator it=music_list->begin();it!=music_list->end();++it)
    {
        qDebug()<<it.key()<<":"<<it.value();
        QString name = it.key();
        ui->show_music_list->addItem(name.toUtf8());
    }
    ui->show_music_list->setCurrentRow(0);

    //timer->start(1000);

    //隐藏播放列表
    ui->show_music_list->hide();
    ui->music_list_title->hide();
    ui->listWidget_lrc->show();
    list_hide = true;
    lrc_hide = true;

    //播放动图
    show_paly_gif();

}

void Music::get_text(QString path)
{
    lrc = new QFile(path,this);
    lrc->open(QFile::ReadOnly);
    QString text = lrc->readLine();
    while(text != NULL ||!text.isEmpty())
    {
        int m = text.mid(1,2).toInt();
        int s = text.mid(4,2).toInt();
        text = text.replace("\n","");
        text = text.remove(0,text.lastIndexOf("]")+1);
        text_list->insert(m*60+s,text);
        text = lrc->readLine();
    }
    lrc->close();

}



void Music::onTimeOut()
{
    /**
    get_percent_pos //获得文件的播放进度（百分比：0--100）。
    get_time_pos //获得文件的当前位置，以秒为单位，精确到小数位1位。
    get_file_name //获得文件的文件名。
    get_time_length //获得文件的长度，以秒为单位。
    get_meta_album //获得文件的 '专辑' 的元数据。
    get_meta_artist //获得文件的 '艺术家' 的元数据。
    get_meta_comment //获得文件的 '评论' 的元数据。
    get_meta_genre //获得文件的 '流派' 的元数据。
    get_meta_title //获得文件的 '标题' 的元数据。
    get_meta_year //获得文件的 '年份' 的元数据。
    */
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



//显示GIF
void Music::show_paly_gif()
{
    QString img_path = QString("./icon/music_paly2.gif");
    QMovie *movie = new QMovie(img_path);
    movie->setScaledSize( ui->music_play->size());
    ui->music_play->setMovie(movie);
    movie->start();

}



Music::~Music()
{
    delete ui;

}


//读取进程返回的信息
void Music::read_all()
{
       encode();
}

//获取音乐列表的音乐路径
QString Music::get_current_music_path()
{
    QString name = ui->show_music_list->currentItem()->text();
    QMap<QString, QString>::const_iterator it = music_list->find(name);

    QString path = it.value();
    return path;


}

//获取音乐信息
void Music::get_music_info()
{
    play->write("get_time_pos\n");
    play->write("get_time_length\n");
    play->write("get_meta_artist\n");
    play->write("get_file_name\n");
}

//播放另一首
void Music::player_nay(QString path)
{
    QString music_path = QString("loadfile %1\n").arg(path);
    QString lrc_name  = path.remove(path.size()-3,path.size())+"lrc";
    qDebug()<<music_path;
    play->write(music_path.toUtf8());
    get_text(lrc_name);
    get_music_info();

}

//启动播放器，并播放第一首
void Music::start_player()
{


    QString music_path = QString("mplayer -slave -quiet volume %1 1 %2")
            .arg(ui->volume->value()).arg(get_current_music_path());
    play->start(music_path);
    qDebug()<<music_path;
    get_music_info();
    timer->start(1000);

    QString name = get_current_music_path();
    QString lrc_name  = name.remove(name.size()-3,name.size())+"lrc";
    get_text(lrc_name);




}

void Music::on_pushButton_mid_clicked()
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



//解析mplayer的回文
void Music::encode()
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

            QMap<int, QString>::const_iterator it = text_list->find(totalTimeNum);
            if(it != text_list->end() && it.key() == totalTimeNum)
            {

                QString text = it.value();
                if(!text.isEmpty())
                {
//
                    QListWidgetItem *item = new QListWidgetItem(text,ui->listWidget_lrc);
//                    item.setText(text);
                    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignBottom);
                    ui->listWidget_lrc->addItem(item);
                    ui->listWidget_lrc->setCurrentRow(ui->listWidget_lrc->count()-1);
                }

            }
//            qDebug()<<"-------"<<totalTimeNum<<":"<<musicTotalTime;
            if(totalTimeNum >= musicTotalTime&&musicTotalTime>0)
                on_pushButton_next_clicked();

        }

        //获取当前歌名
        if(byte.startsWith("ANS_FILENAME"))
        {
//            qDebug()<<info;
            QString title = info.remove(0,14);
            ui->music_name->setText(title.remove(title.size()-5,title.size()));
//            qDebug()<<title;
        }

        //获取当前艺术家姓名
        if(byte.startsWith("ANS_META_ARTIST"))
        {
//            qDebug()<<info;
            QString title = info.remove(0,17);
            ui->label_artist->setText(title.remove(title.size()-5,title.size()));
//            qDebug()<<title;
        }

    }
}


//退出
void Music::on_pushButton_back_clicked()
{
    //发送 结束信号
    mid_flag = false;
    state = false;
    play->kill();
    this->close();
}

//调整声音
void Music::on_volume_valueChanged(int value)
{
    QString num = QString("volume %1 1\n").arg(value);
    play->write(num.toUtf8());
    qDebug()<<"volume :"<<num;

}

//快进，后退停止定时器
void Music::on_progress_bar_sliderPressed()
{
    timer->stop();
}
//快进，后退
void Music::on_progress_bar_sliderReleased()
{

    timer->start(1000);
    QString num = QString("seek %1 2\n").arg(ui->progress_bar->value());
    play->write(num.toUtf8());
    qDebug()<<num;
    QString img_path = QString(":/icon/play.png");
    ui->pushButton_mid->setIconSize(ui->pushButton_mid->size());
    ui->pushButton_mid->setIcon(QIcon(img_path));

    mid_flag = true;

//    ui->textBrowser_lrc->clear();
    ui->listWidget_lrc->clear();

}

//遍历音频文件
bool  Music::FindFile(const QString &path)
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
        else if(filename.contains(".mp3",Qt::CaseSensitive)|filename.contains(".wma",Qt::CaseSensitive))
        {
            ++m_nFilesCount;
            music_list->insert(fileinfo.fileName(),fileinfo.filePath());
//            qDebug()<<fileinfo.filePath();
//            qDebug()<<fileinfo.fileName();
        }
        ++i;

    }while(i < list.size());


    return true;

}
//上一首
void Music::on_pushButton_up_clicked()
{
    int current = ui->show_music_list->currentRow();
    if(current == 0)
        current = ui->show_music_list->count()-1;
    else
        current--;
    ui->show_music_list->setCurrentRow(current);
}

//下一首
void Music::on_pushButton_next_clicked()
{
    int current = ui->show_music_list->currentRow();
    if(current == ui->show_music_list->count()-1)
        current = 0;
    else
        current++;
    ui->show_music_list->setCurrentRow(current);
}

void Music::music_end()
{
    text_list->clear();
//    ui->textBrowser_lrc->clear();
    ui->listWidget_lrc->clear();
    lrc->close();
}

//歌曲列表点击时，播放指定音乐
void Music::on_show_music_list_currentRowChanged(int currentRow)
{
//    state = false;
    if(state == true)
    {

        music_end();
        qDebug()<<"on_show_music_list_currentRowChanged";
        player_nay(get_current_music_path());
        get_music_info();
    }
//    else {
//        state = true;
//        mid_flag = true;
//        start_player();
//        QString img_path = QString(":/icon/play.png");
//        ui->pushButton_mid->setIconSize(ui->pushButton_mid->size());
//        ui->pushButton_mid->setIcon(QIcon(img_path));

//    }

}
////歌曲列表点击时，播放指定音乐
//void Music::on_show_music_list_activated(const QModelIndex &index)
//{
//    state = true;
//    mid_flag = true;
//    start_player();
//    QString img_path = QString(":/icon/play.png");
//    ui->pushButton_mid->setIconSize(ui->pushButton_mid->size());
//    ui->pushButton_mid->setIcon(QIcon(img_path));
//}

void Music::on_pushButton_lrc_clicked()
{
    if(lrc_hide)
    {
        ui->listWidget_lrc->hide();
        lrc_hide = false;
    }else
    {
        ui->listWidget_lrc->show();
        lrc_hide = true;
    }
}

void Music::list_onTimeOut()
{
    if(list_time_flag == true)
    {
        list_time->stop();
        list_time_flag = false;
    }
    ui->show_music_list->hide();
    ui->music_list_title->hide();
    list_hide = true;
}


void Music::on_pushButton_list_clicked()
{
    if(list_hide)
    {
        ui->show_music_list->show();
        ui->music_list_title->show();
        list_hide = false;
        list_time->start(5000);
        list_time_flag = true;
    }else
    {

        list_onTimeOut();
    }
}

