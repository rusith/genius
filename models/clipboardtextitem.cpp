#include "clipboardtextitem.h"

ClipboardTextItem::ClipboardTextItem(QClipboard *clipboard)
{
  _type=ClipboardMimeType::Text;
  if(clipboard && clipboard->mimeData()->hasText())
    this->text(clipboard->text());
  else
    _text=new QString("");
}

ClipboardTextItem::ClipboardTextItem(QString &text)
{
  _type=ClipboardMimeType::Text;
  this->text(text);
}

ClipboardTextItem::ClipboardTextItem(QString *text)
{
  _type=ClipboardMimeType::Text;
  this->text(*text);
}

void ClipboardTextItem::text(QString &text)
{
  deleteTextFile();
  if(text.length()>GSettings::inMemoryTextLength)
  {
    QFile *file=new QFile(QString(Controller::tempFolder.path()+"/%1TEXT.genius").arg(_ref));
    file->open(QFile::WriteOnly);
    qint64 bytwrtn=file->write(text.toUtf8());
    file->close();
    if(bytwrtn<0)
    {
      qDebug()<<"cannot write new text to file ClipboardTextItem::text(QString text)";
      delete file;
      _constructed=false;
      return;
    }
    else
      _textFile=file;
  }
  else
    _text=new QString(text);
}

QString *ClipboardTextItem::text()
{

}


void ClipboardTextItem::deleteTextFile()
{
  if(_textFile)
  {
    if(_textFile->exists())
    {
      if(!_textFile->remove())
      {
        qDebug()<<"cannot delete text file ClipboardTextItem::deleteTextFile()";
      }
      delete _textFile;
      _textFile=NULL;
    }
    else
    {
      delete _textFile;
      _textFile=NULL;
    }

  }
}
