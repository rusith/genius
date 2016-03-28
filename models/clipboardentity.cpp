#include "clipboardentity.h"

int ClipboardEntity::refID=0;

ClipboardEntity::ClipboardEntity(QClipboard *clipboard)
{
  if(clipboard)
  {
    refID++;
    _ref=refID;
    QString FN=Resources::tempFolder.path()+QString("/%1_data.genius").arg(_ref);
    _dataFile=new DataFile(clipboard->mimeData(),FN);
    //setContent(_dataFile);
    _addedTime=new QTime(QTime::currentTime());
  }
}

ClipboardEntity::ClipboardEntity(const QMimeData *mimeData)
{
  if(mimeData)
  {
    refID++;
    _ref=refID;
    QString FN=Resources::tempFolder.path()+QString("/%1_data.genius").arg(_ref);
    _dataFile=new DataFile(mimeData,FN);
    //setContent(_dataFile);
    _addedTime=new QTime(QTime::currentTime());
  }
}

ClipboardEntity::~ClipboardEntity()
{
  if(_dataFile)
    delete _dataFile;
  if(_addedTime)
    delete _addedTime;
  if(_image)
    delete _image;
  if(_plainText)
    delete _plainText;
  if(_HTMLText)
    delete _HTMLText;
}

int ClipboardEntity::ref()
{
  return _ref;
}

bool ClipboardEntity::identical(ClipboardEntity *entity)
{
  if(entity)
    return _dataFile->identicalData(entity->_dataFile);
  else
    return false;
}

QMimeData *ClipboardEntity::data()
{
  if(_dataFile)
    return _dataFile->toMimeData();
  else
    return NULL;
}

bool ClipboardEntity::hasHTML()
{
  if(_HTMLText)
    return true;
  else return false;
}

bool ClipboardEntity::hasImage()
{
  if(_image)
    return true;
  else return false;
}

bool ClipboardEntity::hasPlainText()
{
  if(_dataFile)
    return _dataFile->hasPlainText();
  else return false;
}

const QString *ClipboardEntity::plainText()
{
  return _plainText;
}

QString ClipboardEntity::plainText(bool check, int length)
{
  if(_dataFile)
    return _dataFile->plainText(check,length);
  else
    return "";
}

const QString *ClipboardEntity::HTMLText()
{
  return _HTMLText;
}
const QImage *ClipboardEntity::image()
{
  return _image;
}

const QTime *ClipboardEntity::addedTime()
{
  return _addedTime;
}

void ClipboardEntity::setContent(DataFile *file)
{
  if(file->hasImage())
  {
    if(_image) delete _image;
    QImage *imgTmp=file->image(false);
    if(imgTmp)
    {
      _image=new QImage(imgTmp->scaled(GSettings::maximumImageWidth,GSettings::maximumImageWidth,Qt::KeepAspectRatio));
      delete imgTmp;
    }
  }
  if(file->hasHtmlText())
  {
    if(_HTMLText)delete _HTMLText;
    _HTMLText=new QString(file->HTMLText(false).left(GSettings::inMemoryTextLength));
  }
//  if(file->hasPlainText())
//  {
//    if(_plainText) delete _plainText;
//    _plainText=new QString(file->plainText(false).left(GSettings::inMemoryTextLength));
//  }
}

