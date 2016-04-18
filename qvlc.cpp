#include "qvlc.h"

QVlc::QVlc(QObject *parent) :
    QObject(parent)
{
     vlcInst = 0;
     vlcMedia = 0;
     vlcPlayer = 0;
     rtspAddr = "";
}

bool QVlc::Open(QLabel *lab, QString rtspAddr)
{
    bool ok = true;
    if (ok) {
        this->lab = lab;
        this->rtspAddr = rtspAddr;
        const char *vlc_args[] = {"-I",
                                  "-vv",
                                  "dummy",
                                  "--rtsp-tcp",
                                  "--ignore-config",

                                 };
        vlcInst = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
        //vlcInst = libvlc_new(0, NULL);
        vlcMedia = libvlc_media_new_location(vlcInst, rtspAddr.toLatin1().constData());

        if (vlcInst != 0 && vlcMedia != 0) {
            vlcPlayer = libvlc_media_player_new_from_media(vlcMedia);
            libvlc_release(vlcInst);
            libvlc_media_release(vlcMedia);

#if defined(Q_OS_MAC)
            libvlc_media_player_set_nsobject(vlcPlayer, lab->winId());
#elif defined(Q_OS_UNIX)
            libvlc_media_player_set_xwindow(vlcPlayer, lab->winId());
#elif defined(Q_OS_WIN)
            libvlc_media_player_set_hwnd(vlcPlayer, (void *)lab->winId());
#endif

            ok = true;
        } else {
            ok = false;
        }
    }
    return ok;
}

void QVlc::SetDelayTime(int delayTime)
{
    SetOption(QString(":network-caching=%1").arg(delayTime));
}

void QVlc::Save(QString videoFilePath)
{
    SetOption(QString(":sout=#duplicate{dst=display,dst=std{access=file,mux=ts,dst=%1}}")
              .arg(videoFilePath));
}

void QVlc::SetWidthHeight(int width, int height)
{
    QString args = QString("%1:%2").arg(width).arg(height);
        QByteArray ba = args.toLatin1();
        const char *arg = ba.constData();

        if (vlcPlayer != 0) {
            libvlc_video_set_aspect_ratio(vlcPlayer, arg);
        }
}

bool QVlc::Play()
{
    if (vlcPlayer != 0) {
        libvlc_media_player_play(vlcPlayer);
        return true;
    }
    return false;
}

bool QVlc::IsLive()
{
    bool isLive = false;
    if (vlcPlayer == 0) {
        isLive = false;
    } else {
        isLive = libvlc_media_player_is_playing(vlcPlayer);
    }
    return isLive;
}

void QVlc::Close()
{
    if (vlcPlayer != 0) {
        libvlc_media_player_release(vlcPlayer);
        //释放vlcPlayer对象后重新赋值为0,否则会遇到删除出错的情况
        vlcPlayer = 0;
        //qDebug() << "close ipc ok:" << rtspAddr;
    } else {
        //qDebug() << "close ipc error:" << rtspAddr;
    }
}

void QVlc::SetOption(QString args)
{
    QByteArray ba = args.toLatin1();
        const char *arg = ba.constData();

        if (vlcMedia != 0) {
            libvlc_media_add_option(vlcMedia, arg);
        }
}

bool QVlc::Snapshot(QString imageFilePath)
{
    bool ok = false;
    if (vlcPlayer != 0) {
        int result = libvlc_video_take_snapshot(vlcPlayer, 0,
                                                imageFilePath.toLatin1().constData(),
                                                1920, 1080);;
        if (result == 0) {
            ok = true;
        }
    }
    return ok;
}
