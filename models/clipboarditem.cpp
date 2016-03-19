#include "clipboarditem.h"

int ClipboardItem::clipboardItemRef=0;

ClipboardItem::ClipboardItem()
{
  clipboardItemRef++;
  _ref=clipboardItemRef;
}

ClipboardItem::ClipboardItem(QClipboard *clipboard)
{
  clipboardItemRef++;
  _ref=clipboardItemRef;
  setContent(clipboard);
}

ClipboardItem::~ClipboardItem()
{
  if(_text)
  {
    delete _text;
  }
  if(_image)
  {
    delete _image;
  }
  if(_urls)
  {
    delete _urls;
  }
}

void ClipboardItem::setContent(QClipboard *clipboard)
{
  if(clipboard)
  {
    const QMimeData *mimeData=clipboard->mimeData(QClipboard::Clipboard);
    if(mimeData->hasText())
    {
      if(_text)
        delete _text;

      _text=new QString(mimeData->text());
      _type=Text;
      setTimeToNow();
    }
    else if(mimeData->hasImage())
    {
      if(_image)
        delete _image;

      _image=new QImage(clipboard->image(QClipboard::Clipboard));
      _type=Image;
      setTimeToNow();
    }
    else if(mimeData->hasHtml())
    {
      if(_text)
        delete _text;

      _text=new QString(mimeData->html());
      _type=Text;
      setTimeToNow();
    }
    else if(mimeData->hasUrls())
    {
      if(_urls)
        delete _urls;

      _urls=new QList<QUrl>(mimeData->urls());
      _type=URLs;
      setTimeToNow();
    }
    else {return;}
  }
}

void ClipboardItem::setTimeToNow()
{
  _addedTime=QTime::currentTime();
}

//getters

QImage *ClipboardItem::image()
{
    return _image;
}

QString *ClipboardItem::text()
{
    return _text;
}

QList<QUrl> *ClipboardItem::urls()
{
    return _urls;
}

const QTime *ClipboardItem::addedTime()
{
    return &_addedTime;
}

ClipboardItem::ClipboardMimeType ClipboardItem::type()
{
  return _type;
}

const int &ClipboardItem::ref()
{
  return _ref;
}


//setters

void ClipboardItem::text(QString &str)
{
    *_text=str;
}

void ClipboardItem::image(QImage &image)
{
    *_image=image;
}

void ClipboardItem::urls(QList<QUrl> &urls)
{
    *_urls=urls;
}
