#ifndef RTIMER_H
#define RTIMER_H
#include <QElapsedTimer>
#include <QString>
#include <QDebug>

class RTimer
{
public:
  RTimer(const QString &functionName,const QString &message);
  ~RTimer();
private:
  QElapsedTimer timer_;
  QString FN_;
  QString MSG_;
};

#endif // RTIMER_H
