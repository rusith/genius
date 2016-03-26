#ifndef CLIPBOARDURLITEM_H
#define CLIPBOARDURLITEM_H

#include <QList>
#include <QUrl>
#include <QClipboard>
#include <QMimeData>
#include <models/clipboarditem.h>
/**
 * @brief The ClipboardURLItem class is a type of ClipboardItem . this class holds a list of QUrls
 */
class ClipboardURLItem :public  ClipboardItem
{
public:
  /**
   * @brief main constructor this will construct the object using clipboard reference
   * @param clipboard reference
   */
  ClipboardURLItem(QClipboard *clipboard);

  /**
   * @brief this constructor construct the object using existing list of QUrls
   * @param list of urls
   */
  ClipboardURLItem(const QList<QUrl> &urls);

  /**
   * @brief this constructor construct the object using existing list of QUrls
   * @param list of urls
   */
  ClipboardURLItem(QList<QUrl> *urls);

  ~ClipboardURLItem();

  /**
   * @brief set list of urls
   * @param urls to set
   */
  void urls(const QList<QUrl> &urls);

  /**
   * @brief get list of urls
   * @return list of urls
   */
  QList<QUrl> *urls();

  /**
   * @brief get urls as string seperated with specifid seperator
   * @param seperator for seperate urls
   * @return urls as QString
   */
  QString toString(const QString &seperator);

private:
  /**
   * @brief list of urls
   */
  QList<QUrl> *_urls=NULL;
};

#endif // CLIPBOARDURLITEM_H
