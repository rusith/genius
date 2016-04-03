#ifndef TOOLKIT_H
#define TOOLKIT_H
#include <QString>
#include <QList>
#include <QUrl>
#include <QList>
#include <tools/rtimer.h>
#include <models/filing/fragmentframe.h>

class ToolKit
{
public:
  ToolKit();
  static void removeNewLines(QString *text);
  static QString URlsToString(QList<QUrl> *urls,const QString &seperator="|",bool numbers=false,const QString &numFormat="%1. ");
  static QString URLsToPreviewText(QList<QUrl> *urls, const uint &maxLenght=0);
  static FragmentFrame maxValue(const QList<FragmentFrame> &frames);
  static void Sort(QList<FragmentFrame> *frames);
};

#endif // TOOLKIT_H
