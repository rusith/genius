#ifndef TOOLKIT_H
#define TOOLKIT_H
#include <QString>
#include <QList>
#include <QUrl>

class ToolKit
{
public:
  ToolKit();
  static void removeNewLines(QString *text);
  static QString URlsToString(QList<QUrl> *urls);
};

#endif // TOOLKIT_H
