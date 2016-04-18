#ifndef QVLC_H
#define QVLC_H

#include <QObject>
#include "vlc/vlc.h"
#include <QLabel>


class QVlc : public QObject
{
    Q_OBJECT
public:
    explicit QVlc(QObject *parent = 0);

    bool Open(QLabel *lab, QString rtspAddr);       //打开视频流
    void SetDelayTime(int delayTime);               //设置缓存时长,单位毫秒
    void Save(QString videoFilePath);               //设置保存录像文件
    void SetWidthHeight(int width, int height);     //设置视频宽高比例
    bool Play();                                    //播放视频流
    bool IsLive();                                  //检测是否活着
    bool Snapshot(QString imageFilePath);           //保存快照
    void Close();                                   //关闭视频流

    QString RtspAddr()const {
        return rtspAddr;
    }

    void SetRtspAddr(QString rtspAddr) {
        this->rtspAddr = rtspAddr;
    }

private:
    libvlc_instance_t *vlcInst;
    libvlc_media_t *vlcMedia;
    libvlc_media_player_t *vlcPlayer;

    QLabel *lab;
    QString rtspAddr;
    void SetOption(QString args);
};

#endif // QVLC_H
