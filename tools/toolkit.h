#ifndef TOOLKIT_H
#define TOOLKIT_H
#include <QString>
#include <QList>
#include <QUrl>
#include <models/filing/fragmentframe.h>
#include <QList>

class ToolKit
{
public:
  ToolKit();
  static void removeNewLines(QString *text);
  static QString URlsToString(QList<QUrl> *urls);
  const static FragmentFrame maxValue(const QList<FragmentFrame> &frames);
};

#endif // TOOLKIT_H
