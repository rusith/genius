#include "clipboarditem.h"

int ClipboardItem::clipboardItemRef=1;

ClipboardItem::ClipboardItem()
{
  clipboardItemRef=clipboardItemRef+1;
  _ref=clipboardItemRef;
  _addedTime=new QTime(QTime::currentTime());
  _constructed=true;
}

ClipboardItem::~ClipboardItem()
{
  if(_addedTime)
    delete _addedTime;
}


ClipboardItem::ClipboardMimeType ClipboardItem::type()
{
  return _type;
}

int ClipboardItem::ref()
{
  return _ref;
}

bool ClipboardItem::constructed()
{
  return _constructed;
}

QTime *ClipboardItem::addedTime()
{
  return _addedTime;
}


//int ClipboardItem::clipboardItemRef=0;

//ClipboardItem::ClipboardItem()
//{
//  clipboardItemRef++;
//  _ref=clipboardItemRef;

//}

//ClipboardItem::ClipboardItem(QClipboard *clipboard)
//{
//  clipboardItemRef++;
//  _ref=clipboardItemRef;
//  setContent(clipboard);
//}

//ClipboardItem::~ClipboardItem()
//{
//  if(_text)
//  {
//    delete _text;
//  }
//  if(_image)
//  {
//    delete _image;
//  }
//  if(_imageFile)
//  {
//    if(_imageFile->exists())
//    {
//      _imageFile->remove();
//    }
//    delete _imageFile;
//  }

//  if(_urls)
//  {
//    delete _urls;
//  }
//}

//void ClipboardItem::setContent(QClipboard *clipboard)
//{
//  if(clipboard)
//  {
//    const QMimeData *mimeData=clipboard->mimeData(QClipboard::Clipboard);
//    _constructedSuccessfully=true;
//    if(mimeData->hasText())
//    {
//      if(_text)
//        delete _text;

//      _text=new QString(mimeData->text());
//      _type=Text;
//      setTimeToNow();
//    }
//    else if(mimeData->hasImage())
//    {
//      if(_image)
//        delete _image;

//      QImage image=clipboard->image();
//      _imageHight=image.height();
//      _imageWidth=image.width();
//      int pixels=(_imageHight*_imageHight);
//      qDebug()<<pixels;
//      if(pixels<1)
//      {
//        _constructedSuccessfully=false;
//        return;
//      }

//      if(pixels>10000)
//      {
//        if(_imageFile)
//        {
//          if(_imageFile->exists())
//          {
//            _imageFile->remove();
//          }
//          delete _imageFile;
//        }
//        _imageFile=new QFile(Resources::tempDir.path()+QString("/%1IMG.png").arg(_ref));
//        image.save(_imageFile->fileName());
//        _inFile=true;

//        if(_image)
//          *_image=image.scaled(100,100,Qt::KeepAspectRatio);
//        else
//          _image=new QImage(image.scaled(100,100,Qt::KeepAspectRatio));
//      }
//      else
//      {
//        if(_image)
//        {

//          *_image=image;
//        }
//        else
//          _image=new QImage(image);
//      }

//      _type=Image;
//      setTimeToNow();
//    }
//    else if(mimeData->hasHtml())
//    {
//      if(_text)
//        delete _text;

//      _text=new QString(mimeData->html());
//      _type=Text;
//      setTimeToNow();
//    }
//    else if(mimeData->hasUrls())
//    {
//      if(_urls)
//        delete _urls;

//      _urls=new QList<QUrl>(mimeData->urls());
//      _type=URLs;
//      setTimeToNow();
//    }
//    else {return;}
//  }
//}

//void ClipboardItem::setTimeToNow()
//{
//  _addedTime=QTime::currentTime();
//}

////getters

//QImage ClipboardItem::image()
//{
//  if(_inFile)
//  {
//    if(_imageFile)
//    {
//      QImage img(_imageFile->fileName());
//      return img;
//    }
//    else
//    {
//      return QImage();
//    }
//  }
//  else
//  {
//    return *_image;
//  }
//}

//QString *ClipboardItem::text()
//{
//    return _text;
//}

//QList<QUrl> *ClipboardItem::urls()
//{
//    return _urls;
//}

//const QTime *ClipboardItem::addedTime()
//{
//    return &_addedTime;
//}

//ClipboardItem::ClipboardMimeType ClipboardItem::type()
//{
//  return _type;
//}

//const int &ClipboardItem::ref()
//{
//  return _ref;
//}

//int ClipboardItem::imageHight()
//{
//  return _imageHight;
//}

//int ClipboardItem::imageWidth()
//{
//  return _imageWidth;
//}

//bool ClipboardItem::constructedSuccessfully()
//{
//  return _constructedSuccessfully;
//}

//QImage *ClipboardItem::imagePreview()
//{
//  return _image;
//}

////setters

//void ClipboardItem::text(QString &str)
//{
//    *_text=str;
//}

//void ClipboardItem::image(QImage &image)
//{
//  if((image.width()*image.height())>10000)
//  {
//    if(_inFile)
//    {
//      image.save(_imageFile->fileName());
//    }
//    else
//    {
//      QFile file(Resources::tempDir.path()+QString("/%1IMG.png").arg(_ref));
//      image.save(file.fileName());
//      _imageFile->remove();
//      delete _imageFile;
//      _imageFile=new QFile(file.fileName());
//      _inFile=true;
//    }

//    if(_image)
//      *_image=image.scaled(100,100,Qt::KeepAspectRatio);
//    else
//      _image=new QImage(image.scaled(100,100,Qt::KeepAspectRatio));
//  }
//  else
//  {
//    _inFile=false;
//    if(_imageFile)
//    {
//      _imageFile->remove();
//      delete _imageFile;
//      _imageFile=NULL;
//    }
//    if(_image)
//      *_image=image;
//    else
//      _image=new QImage(image);
//  }
//}

//void ClipboardItem::urls(QList<QUrl> &urls)
//{
//    *_urls=urls;
//}


