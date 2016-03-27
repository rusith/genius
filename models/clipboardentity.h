#ifndef CLIPBOARDENTITY_H
#define CLIPBOARDENTITY_H

#include <QClipboard>
#include <QMimeData>
#include <models/filing/datafile.h>
#include <resources/resources.h>

class ClipboardEntity
{
public:
  ClipboardEntity(QClipboard *clipboard);
  ClipboardEntity(const QMimeData *mimeData);
  ~ClipboardEntity();

  int ref();
  static int refID;
private:
  int _ref;

  DataFile *_dataFile=NULL;
};

#endif // CLIPBOARDENTITY_H
