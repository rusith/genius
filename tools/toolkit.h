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
  /**
   * @brief remove an item from a list
   * @param list for use
   * @param item to be removed(if the item is not availabel in the list nothing will do )
   */
  template <typename T>
  static void removeItem(QList<T> *list,const T &item)
  {
    if(list && !list->isEmpty())
    {
      int index=list->indexOf(item);
      if(index>-1)
        list->removeAt(index);
    }
  }
};

#endif // TOOLKIT_H
