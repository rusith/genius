#include "clipboardurlitem.h"

ClipboardURLItem::ClipboardURLItem(QClipboard *clipboard)
{
  _type=ClipboardMimeType::URLs;

  if(clipboard && clipboard->mimeData()->hasUrls())
  {
     _urls=new QList<QUrl>(clipboard->mimeData()->urls());
  }
  else
    _constructed=false;
}

ClipboardURLItem::ClipboardURLItem(const QList<QUrl> &urls)
{
  _type=ClipboardMimeType::URLs;
  _urls=new QList<QUrl>(urls);
}


ClipboardURLItem::ClipboardURLItem(QList<QUrl> *urls)
{
  _type=ClipboardMimeType::URLs;
  if(urls)
  {
    _urls=new QList<QUrl>(*urls);
  }
  else
    _constructed=false;
}


ClipboardURLItem::~ClipboardURLItem()
{
  if(_urls)
    delete _urls;
}


void ClipboardURLItem::urls(const QList<QUrl> &urls)
{

  if(_urls)
    delete _urls;
  _urls=new QList<QUrl>(urls);

}

QList<QUrl> *ClipboardURLItem::urls()
{
  return _urls;
}

QString ClipboardURLItem::toString(const QString &seperator)
{
  if(_urls)
  {
    QString text;
    if(_urls->length()>0)
    {
      int i=0;
      foreach (QUrl url, *_urls)
      {
        text.append((i!=0?" "+seperator+" ":"")+url.toString()+"\n");
        i++;
      }
      return text;
    }
    else
      return text;
  }
  else return "";
}

