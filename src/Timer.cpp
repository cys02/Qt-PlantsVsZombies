//
// Created by sun on 9/2/16.
//

#include "Timer.h"

Timer::Timer(QObject *parent, int timeout, std::function<void(void)> functor) : QTimer(parent)
{
    setInterval(timeout);
    if (timeout < 50)
        setTimerType(Qt::PreciseTimer);
    setSingleShot(true);
    connect(this, &Timer::timeout, [this, functor] { functor(); deleteLater(); });
    //绑定timeout信号与lambda函数,一次性函数，用完即析构
    //信号的发送者；发送的信号（函数的地址）；信号的接收者；接收后进行的动作
}

TimeLine::TimeLine(QObject *parent, int duration, int interval, std::function<void(qreal)> onChanged, std::function<void(void)> onFinished, CurveShape shape)
        : QTimeLine(duration, parent)
{
    if (duration == 0) {
        int i = 1;
        ++i;
    }
    setUpdateInterval(interval);
    setCurveShape(shape);
    connect(this, &TimeLine::valueChanged, onChanged);
    connect(this, &TimeLine::finished, [this, onFinished] { onFinished(); deleteLater(); });
}
