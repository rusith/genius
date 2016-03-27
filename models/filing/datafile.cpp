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
}size

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
  _file->open(QFile::ReadOnly);
  _file->seek(frame.start);
  char *cha=new char[frame.size];
  _file->read(cha,frame.size);
  _file->close();
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
      _file->open(QFile::ReadOnly);
      _file->seek(frame.start);
      char *cha=new char[frame.size];
      _file->read(cha,frame.size);
      _file->close();
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

