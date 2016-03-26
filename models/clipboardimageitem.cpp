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
    _constructed=image(image);
  _type=ClipboardMimeType::Image;
}

ClipboardImageItem::ClipboardImageItem(QImage &image)
{
  _constructed=image(&image);
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
    deleteImageFile();

    if(!CanStoreOnMemory(image))
    {
      QFile file=new QFile(QString(Controller::tempFolder.path()+"/%1IMG.png").arg(_ref));
      bool flSvd=image->save(file.fileName());
      if(flSvd)
        _imageFile=file;
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
        _image=img;
    }
    else
    {
      if(_image)
        delete _image;
      _image=new QImage(*image);
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
  return new QImage();
}

bool ClipboardImageItem::CanStoreOnMemory(QImage *image)
{
  if(image)
  {
    if(image->width()>GSettings::maximumImageWidth || image->height()>GSettings::maximumImageHight)
      return false;
    else return true;\
  }
}
