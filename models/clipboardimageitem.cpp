#include "clipboardimageitem.h"

ClipboardImageItem::ClipboardImageItem(QClipboard *clipboard)
{
  if(clipboard->mimeData()->hasImage())
  {
    _constructed=image(new QImage(clipboard->image()));
    _type=ClipboardMimeType::Image;
  }
}

ClipboardImageItem::ClipboardImageItem(QImage *image)
{
  if(image)
    _constructed=this->image(image);
  _type=ClipboardMimeType::Image;
}

ClipboardImageItem::ClipboardImageItem(const QImage &image)
{
  QImage img(image);
  _constructed=this->image(&img);
  _type=ClipboardMimeType::Image;
}

ClipboardImageItem::~ClipboardImageItem()
{
  deleteImageFile();
  if(_image)
    delete _image;
}

void ClipboardImageItem::deleteImageFile()
{
  if(_imageFile)
  {
    if(_imageFile->exists())
    {
      if(!_imageFile->remove())
      {
        qDebug()<<"cannot remove existing image file";
      }
      delete _imageFile;
      _imageFile=NULL;
    }
    else
    {
      delete _imageFile;
      _imageFile=NULL;
    }
  }
}

bool ClipboardImageItem::image(QImage *image)
{
  if(image)
  {
    int W=image->width();
    int H=image->height();
    if(W<1 || H<1)
    {
      return false;
    }
    if(!CanStoreOnMemory(image))
    {
      QFile *file=new QFile(QString(Resources::tempFolder.path()+"/%1IMG.png").arg(_ref));
      bool flSvd=image->save(file->fileName());
      if(flSvd)
      {
        if(!(_imageFile && _imageFile->fileName()==file->fileName()))
        {
          deleteImageFile();
          _imageFile=file;
        }

      }
      else
      {
        qDebug()<<"cannot save image ClipboardImageItem::setImage(QImage &image)";
        return false;
      }
      QImage *img =new QImage(image->scaled(GSettings::maximumImageWidth,GSettings::maximumImageHight,Qt::KeepAspectRatio));
      if(img)
      {
        if(_image)
          delete _image;
        _image=img;
      }
      else
        _image=new QImage;


      _width=W;
      _hight=H;
      return true;
    }
    else
    {
      deleteImageFile();
      if(_image)
        delete _image;
      _image=new QImage(*image);

      _width=W;
      _hight=H;
      return true;
    }
  }
  else
    return false;
}

QImage *ClipboardImageItem::image()
{
  if(_imageFile)
  {
    if(_imageFile->exists())
    {
      return new QImage(_imageFile->fileName());
    }
    return new QImage;
  }
  else
  {
    if(_image)
      return new QImage(*_image);
    else
      return new QImage;
  }
}

QImage *ClipboardImageItem::preview()
{
  if(_image)
    return _image;
  return NULL;
}

bool ClipboardImageItem::CanStoreOnMemory(QImage *image)
{
  if(image)
  {
    if(image->width()>GSettings::maximumImageWidth || image->height()>GSettings::maximumImageHight)
      return false;
    else return true;
  }
  else
    return false;
}

int ClipboardImageItem::width()
{
  return _width;
}

int ClipboardImageItem::hight()
{
  return _hight;
}
