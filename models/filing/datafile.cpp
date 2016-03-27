#include "datafile.h"

DataFile::DataFile(const QMimeData *mimeData, const QString &filename)
{
  _fragments=new QHash<QString,FragmentFrame>();
  if(mimeData)
  {
    if(mimeData->formats().count()>0)
    {
      _file=new QFile(filename);
      _file->open(QFile::WriteOnly);
      foreach (QString format, mimeData->formats())
      {
        FragmentFrame frame;
        frame.start=_file->pos();
        qint64 wrtn=_file->write(mimeData->data(format));
        frame.end=_file->pos();
        frame.size=wrtn;
        _fragments->insert(format,frame);
      }
      _file->close();
    }
  }
}

DataFile::~DataFile()
{
  if(_file)
    if(_file->exists())
      _file->remove();
    delete _file;
  if(_fragments)
    delete _fragments;
}

QByteArray *DataFile::data(const QString &format)
{
  if(!_file || !_fragments || _fragments->isEmpty())
    return NULL;
  FragmentFrame frame=_fragments->value(format);
  if(frame.end==0 && frame.start==0 && frame.size==0)
    return new QByteArray(0);
  if(frame.size==0)
    return new QByteArray(0);
  char *cha=new char[frame.size];
  readFragment(frame,cha);
  QByteArray *BA=new QByteArray(cha,frame.size);
  delete [] cha;
  return BA;
}

QByteArray *DataFile::at(const int &index)
{
  if(!_file || !_fragments ||_fragments->isEmpty() ||index<0 ||index>_fragments->count()-1)
    return NULL;
  int in=0;
  foreach (FragmentFrame frame, *_fragments)
  {
    if(in==index)
    {
      char *cha=new char[frame.size];
      readFragment(frame,cha);
      QByteArray *BA=new QByteArray(cha,frame.size);
      delete [] cha;
      return BA;
    }
    in++;
  }
  return NULL;
}

bool DataFile::isEmpty()
{
  return _fragments->isEmpty();
}

int DataFile::count()
{
  return _fragments->count();
}

QString DataFile::fileName()
{
  if(_file)
  {
    return _file->fileName();
  }
  else
    return QString();
}

QMimeData *DataFile::toMimeData()
{
  if(_fragments && count()>0 && _file && _file->exists())
  {
    QMimeData *MD=new QMimeData();
    _file->open(QFile::ReadOnly);
    foreach (QString key, _fragments->keys())
    {
      FragmentFrame frame=_fragments->value(key);
      if(frame.end==0 && frame.start==0 && frame.size==0)
        continue;
      _file->seek(frame.start);
      char *cha=new char[frame.size];
      _file->read(cha,frame.size);
      MD->setData(key,QByteArray(cha,frame.size));
      delete [] cha;
    }
    _file->close();
    return MD;
  }
  else
    return NULL;
}

bool DataFile::hasPlainText()
{
  if(_fragments && count()>0 && _file && _file->exists())
  {
    foreach (QString str, _fragments->keys())
    {
       if(str=="text/plain"||str=="text/plain;charset=utf-8")
         return true;
    }
    return false;
  }
  else
    return false;
}

bool DataFile::hasHtmlText()
{
  QStringList list;
  list.append("text/html");
  return hasFormat(list);
}

bool DataFile::hasImage()
{
  QStringList list;
  list.append("image/png");
  list.append("image/bmp");
  list.append("image/x-bmp");
  list.append("image/x-MS-bmp");
  list.append("image/jpeg");
  list.append("image/x-icon");
  list.append("image/x-ico");
  list.append("image/x-win-bitmap");
  list.append("image/tiff");
  list.append("application/x-qt-image");
  return hasFormat(list);
}


bool DataFile::hasFormat(const QStringList &strlst)
{
  if(_fragments && count()>0 && _file && _file->exists())
  {
    foreach (QString str, strlst)
    {
      if(_fragments->keys().contains(str))
        return true;
    }
    return false;
  }
  else
    return false;
}


QString DataFile::plainText(bool check)
{
  if(check)
    if(!hasPlainText())
      return "";
  if(_fragments && count()>0 && _file && _file->exists())
  {
    FragmentFrame frame=_fragments->value("text/plain");
    if(frame.size<1)
    {
      frame=_fragments->value("text/plain;charset=utf-8");
      if(frame.size<1)
        return "";
    }
    char *cha=new char[frame.size];
    readFragment(frame,cha);
     QString str(cha);
     delete [] cha;
     return str;
  }
  else
    return "";
}


QString DataFile::HTMLText(bool check)
{
  if(check)
    if(!hasHtmlText())
      return "";
  if(_fragments && count()>0 && _file && _file->exists())
  {
    FragmentFrame frame=_fragments->value("text/html");
    if(frame.size<1)
      return "";
    char *cha=new char[frame.size];
    readFragment(frame,cha);
     QString str(cha);
     delete [] cha;
     return str;
  }
  else
    return "";
}

QImage *DataFile::image(bool check)
{
  if(check)
    if(!hasImage())
      return NULL;
  if(_fragments && count()>0 && _file && _file->exists())
  {
    QStringList list;
    list.append("image/png");
    list.append("image/bmp");
    list.append("image/x-bmp");
    list.append("image/x-MS-bmp");
    list.append("image/jpeg");
    list.append("image/x-icon");
    list.append("image/x-ico");
    list.append("image/x-win-bitmap");
    list.append("image/tiff");
    list.append("application/x-qt-image");

    foreach (QString str, list)
    {
      if(_fragments->contains(str))
      {
        FragmentFrame frame=_fragments->value(str);
        if(frame.size<1)
          continue;
        else
        {
          char *cha=new char[frame.size];
          readFragment(frame,cha);
           QImage *img= new QImage(QImage::fromData((uchar*)cha,frame.size));
           delete [] cha;
           return img;
        }
      }
    }
    return NULL;
  }
  else
    return NULL;
}

qint64 DataFile::readFragment(const FragmentFrame &frame,char *cha)
{
  _file->open(QFile::ReadOnly);
  _file->seek(frame.start);
  qint64 in=_file->read(cha,frame.size);
  _file->close();
  return in;
}

