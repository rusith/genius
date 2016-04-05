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
    _addedTime=new QTime(QTime::currentTime());
  }
}

ClipboardEntity::~ClipboardEntity()
{
  if(_dataFile)
    delete _dataFile;
  if(_addedTime)
    delete _addedTime;
}

int ClipboardEntity::ref()
{
  return _ref;
}

bool ClipboardEntity::identical(ClipboardEntity *entity)
{
  if(entity)
    return _dataFile->operator ==(entity->_dataFile);
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

QByteArray *ClipboardEntity::data(const QString &format)
{
  if(_dataFile)
    return _dataFile->data(format);
  else return NULL;
}

bool ClipboardEntity::hasHTML()
{
  if(_dataFile)
    return _dataFile->hasHtmlText();
  else return false;
}

bool ClipboardEntity::hasImage()
{
  if(_dataFile)
    return _dataFile->hasImage();
  else return false;
}

bool ClipboardEntity::hasPlainText()
{
  if(_dataFile)
    return _dataFile->hasPlainText();
  else return false;
}


QString ClipboardEntity::plainText(bool check, int length)
{
  if(_dataFile)
    return _dataFile->plainText(check,length);
  else
    return "";
}

const QString ClipboardEntity::HTMLText(bool check, const int &length)
{
  if(_dataFile)
    return _dataFile->HTMLText(check,length);
  else
    return "";
}

QImage *ClipboardEntity::image(bool check,const int &width,const int &hight)
{
  if(_dataFile)
    return _dataFile->image(check,width,hight);
  else
    return NULL;
}

const QTime *ClipboardEntity::addedTime()
{
  return _addedTime;
}

QStringList ClipboardEntity::contentFormats()
{
  if(_dataFile)
    return _dataFile->formats();
  else
    return QStringList();
}

quint32 ClipboardEntity::formatSize(const QString &format)
{
  if(_dataFile)
    return _dataFile->formatSize(format);
  else
    return 0;
}

QStringList ClipboardEntity::imageFormats()
{
  if(_dataFile)
    return _dataFile->imageFormats();
  return QStringList();
}

QStringList ClipboardEntity::formats()
{
  return _dataFile->formats();
}

quint32 ClipboardEntity::size()
{
  return _dataFile->size();
}

bool ClipboardEntity::hasURLs()
{
  return _dataFile->hasUrls();
}

QList<QUrl> ClipboardEntity::urls()
{
  return _dataFile->urls();
}
