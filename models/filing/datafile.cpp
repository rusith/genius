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
        if(format=="text/html")
        {
          qint64 wrtn=_file->write(mimeData->html().toUtf8());
          frame.size=wrtn;
          _fragments->insert(format,frame);
          continue;
        }
        qint64 wrtn=_file->write(mimeData->data(format));
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
  {
    if(_file->exists())
        _file->remove();
    delete _file;
  }
  if(_fragments)
    delete _fragments;
}

QByteArray *DataFile::data(const QString &format) const
{
  if(_fragments->isEmpty())
    return NULL;
  FragmentFrame frame=_fragments->value(format);
  if(frame.start==0 && frame.size==0)
    return new QByteArray(0);
  if(frame.size==0)
    return new QByteArray(0);
  return readFragment(frame);
}

QByteArray *DataFile::at(const int &index)
{
  if(!_file || !_fragments ||_fragments->isEmpty() ||index<0 ||index>_fragments->count()-1)
    return NULL;
  int in=0;
  foreach (FragmentFrame frame, *_fragments)
  {
    if(in==index)
      return readFragment(frame);
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
  if(_fragments && count()>0 && _file)
  {
    QMimeData *MD=new QMimeData();
    _file->open(QFile::ReadOnly);
    foreach (QString key, _fragments->keys())
    {
      FragmentFrame frame=_fragments->value(key);
      if(frame.start==0 && frame.size==0)
        continue;
      _file->seek(frame.start);
      MD->setData(key,_file->read(frame.size));
    }
    _file->close();
    return MD;
  }
  else
    return NULL;
}

bool DataFile::hasPlainText()
{
  return _fragments->keys().contains("text/plain");
}

bool DataFile::hasHtmlText()
{
  return _fragments->keys().contains("text/html");
}

bool DataFile::hasImage()
{
  foreach (QString key, _fragments->keys())
  {
      if (key.startsWith("image"))
        return true;
  }
  return false;

//  QStringList list;
//  list.append("image/*");
//  list.append("image/bmp");
//  list.append("image/x-bmp");
//  list.append("image/x-MS-bmp");
//  list.append("image/jpeg");
//  list.append("image/x-icon");
//  list.append("image/x-ico");
//  list.append("image/x-win-bitmap");
//  list.append("image/tiff");
//  list.append("application/x-qt-image");
//  return hasFormat(list);
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

QString DataFile::plainText(bool check, int length)
{
  if(check)
    if(!hasPlainText())
      return "";
  if(count()>0 && _file && _file->exists())
  {
    FragmentFrame frame=_fragments->value("text/plain");
    if(frame.size<1 || length==0)
        return "";
    if(length!=-1 && (quint64)length<=frame.size)
      frame.size=length;
    QByteArray *BA=readFragment(frame);
    QString str=QString::fromUtf8(*BA);
    delete BA;
    return str;
  }
  else
    return "";
}


QString DataFile::HTMLText(bool check,int length)
{
  if(check)
    if(!hasHtmlText())
      return "";
  if(_fragments && count()>0 && _file && _file->exists())
  {
    FragmentFrame frame=_fragments->value("text/html");
    if(frame.size<1 || length==0)
      return "";
    if(length!=-1 && (quint64)length<=frame.size)
      frame.size=length;
    QByteArray *BA=readFragment(frame);
    QString str=QString::fromUtf8(*BA);
    delete BA;
    return str;
  }
  else
    return "";
}

QImage *DataFile::image(bool check,const int &width, const int &hight)
{
  if(check)
    if(!hasImage())
      return NULL;
  if(count()>0 && _file && _file->exists())
  {
    QList<FragmentFrame> imgs;
    foreach (QString key, _fragments->keys())
    {
      if(key.startsWith("image"))
      {
        FragmentFrame frame=_fragments->value(key);
        imgs.append(frame);
//        if(frame.size<1) continue;
//        QImage *img;
//        if(width>0 && hight>0)
//          img=new QImage(QImage::fromData(*readFragment(frame)).scaled(width,hight,Qt::KeepAspectRatio));
//        else
//          img=new QImage(QImage::fromData(*readFragment(frame)));
//        return img;
      }
      else continue;
    }
    FragmentFrame max=ToolKit::maxValue(imgs);
    if(max.size==0) return NULL;

    QImage *img;
    if(width>0 && hight>0)
    {
      QByteArray *BA=readFragment(max);
      if(BA)
      {
        img=new QImage(QImage::fromData(*BA).scaled(width,hight,Qt::KeepAspectRatio));
        delete BA;
        return img;
      }
      else
        return NULL;
    }
    else
    {
      QByteArray *BA=readFragment(max);
      if(BA)
      {
        img=new QImage(QImage::fromData(*BA));
        delete BA;
        return img;
      }

      else
        return NULL;
      //img=new QImage(QImage::fromData(*readFragment(frame)));
    }
//    foreach (QString str, list)
//    {
//      if(_fragments->contains(str))
//      {
//        else
//        {
//          QImage *img;
//          if(width>0 && hight>0)
//            img=new QImage(QImage::fromData(*readFragment(frame)).scaled(width,hight,Qt::KeepAspectRatio));
//          else
//            img=new QImage(QImage::fromData(*readFragment(frame)));
//          return img;
//        }
//      }
//    }
//    return NULL;
  }
  else
    return NULL;
}

bool DataFile::identicalData(DataFile *file)
{
  if(file)
  {
    if(file->_fragments->count()==this->_fragments->count())
    {
      if(file->_fragments->keys()==this->_fragments->keys())
      {
        foreach (QString key,this->_fragments->keys() )
        {
          if(!(file->_fragments->value(key)==this->_fragments->value(key)))
           return false;
        }

        QByteArray *baFile;
        QByteArray *baThis;
        foreach (QString key,this->_fragments->keys() )
        {
          if(key!="TIMESTAMP")
          {
            baFile=file->readFragment(file->_fragments->value(key));
            baThis=readFragment(_fragments->value(key));
            if(baFile && baThis)
            {
              if(*baFile!=*baThis)
              {
                delete baFile;
                delete baThis;
                return false;
              }
            }
            if(baFile)
              delete baFile;
            if(baThis)
              delete baThis;
          }
        }
        return true;
      }
      else
        return false;
    }
    else
      return false;
  }
  else
    return false;
}

qint64 DataFile::readFragment(const FragmentFrame &frame,char *cha)
{
  _file->open(QFile::ReadOnly);
  _file->seek(frame.start);
  qint64 in=_file->read(cha,frame.size);
  _file->close();
  return in;
}

QByteArray *DataFile::readFragment(const FragmentFrame &frame) const
{
  _file->open(QFile::ReadOnly);
  _file->seek(frame.start);
  QByteArray *BA=new QByteArray( _file->read(frame.size));
  _file->close();
  if(BA->isEmpty())
  {
    delete BA;
    return NULL;
  }
  return BA;
}

QStringList DataFile::formats()
{
  if(_fragments)
    return QStringList(_fragments->keys());
  return QStringList();
}

quint64 DataFile::formatSize(const QString &format)
{
  if(_fragments)
  {
    if(_fragments->keys().contains(format))
      return _fragments->value(format).size;
    else
      return 0;
  }
  else
    return 0;
}

QStringList DataFile::imageFormats()
{
  if(_fragments)
  {
    QStringList res;
    foreach (QString key, _fragments->keys())
    {
      if(key.startsWith("image"))
        res.append(key);
    }
    return res;
  }
  else
    return QStringList();
}


bool DataFile::operator ==(const DataFile &rhs)const
{
  //check has _fragments
  //RTimer t("DataFile::operator ==(const DataFile &rhs)","compairing two DataFiles with data");
    //check has _fragments
  if(_fragments && rhs._fragments)
  {
    if(_fragments->keys()==rhs._fragments->keys())
    {
      QByteArray* baThis;
      QByteArray* baRhs;
      foreach (QString key,this->_fragments->keys() )
      {
        if(key!="TIMESTAMP")
        {
          FragmentFrame fThis=_fragments->value(key);
          FragmentFrame fRhs=rhs._fragments->value(key);
          if(!(fRhs==fThis))
            return false;
          baThis=readFragment(fThis);
          baRhs=rhs.readFragment(fRhs);
          if(baThis)
          {
            if(baRhs)
            {
              if(*baThis!=*baRhs)
              {
                delete baThis;
                delete baRhs;
                return false;
              }
              delete baRhs;
            }
            delete baThis;
          }
        }
      }
      return true;
    }
    else
      return false;
  }
  else
    return false;
}

bool DataFile::operator ==(DataFile *rhs) const
{
  //RTimer t("DataFile::operator ==(DataFile *rhs)","compairing two DataFiles with data");
  if(!rhs)return false;
    //check has _fragments
  if(_fragments && rhs->_fragments)
  {
    if(_fragments->keys()==rhs->_fragments->keys())
    {
      QByteArray* baThis;
      QByteArray* baRhs;
      foreach (QString key,this->_fragments->keys() )
      {
        if(key!="TIMESTAMP")
        {
          FragmentFrame fThis=_fragments->value(key);
          FragmentFrame fRhs=rhs->_fragments->value(key);
          if(!(fRhs==fThis))
            return false;
          baThis=readFragment(fThis);
          baRhs=rhs->readFragment(fRhs);
          if(baThis)
          {
            if(baRhs)
            {
              if(*baThis!=*baRhs)
              {
                delete baThis;
                delete baRhs;
                return false;
              }
              delete baRhs;
            }
            delete baThis;
          }
        }
      }
      return true;
    }
    else
      return false;
  }
  else
    return false;
}
