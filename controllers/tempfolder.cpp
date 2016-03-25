#include "tempfolder.h"

TempFolder::TempFolder(QObject *parent) : QObject(parent)
{
  _path=QDir::temp().path()+"/Genius";
  _folder=QDir(_path);
  createIfNotExists();
}

TempFolder::~TempFolder()
{
  remove();
}

void TempFolder::createIfNotExists()
{
  if(!_folder.exists())
  {
    _folder.mkdir(_folder.path());
    if(!_folder.exists())
    {
      qDebug()<<"cannot create tem folder ("+_folder.path()+")";
    }
  }
}

QString TempFolder::path()
{
  createIfNotExists();
  return _path;
}

void TempFolder::remove()
{
  if(_folder.exists())
  {
    if(!_folder.removeRecursively())
      qDebug()<<"cannot remove file ("+_folder.path()+")";
  }
}
