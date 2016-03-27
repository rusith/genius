#include "clipboardentity.h"

int ClipboardEntity::refID=0;

ClipboardEntity::ClipboardEntity(QClipboard *clipboard)
{
  if(clipboard)
  {
    refID++;
    _ref=refID;
    QString FN=Resources::tempFolder.path()+QString("%1_Data.genius").arg(_ref);
    _dataFile=new DataFile(clipboard->mimeData(),FN);
  }
}

ClipboardEntity::ClipboardEntity(const QMimeData *mimeData)
{
  if(mimeData)
  {
    refID++;
    _ref=refID;
    QString FN=Resources::tempFolder.path()+QString("%1_Data.genius").arg(_ref);
    _dataFile=new DataFile(mimeData,FN);
  }
}

ClipboardEntity::~ClipboardEntity()
{
  if(_dataFile)
    delete _dataFile;
}

