#include "rtimer.h"

RTimer::RTimer(const QString &functionName, const QString &message)
{
  timer_.start();
  FN_=functionName;
  MSG_=message;
}

RTimer::~RTimer()
{
  qDebug()<<QString("the function %1 got %2 ms to do the work  \n %3").arg(FN_).arg(timer_.nsecsElapsed()/1000).arg(MSG_);
}

