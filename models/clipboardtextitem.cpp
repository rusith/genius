#include "clipboardtextitem.h"

ClipboardTextItem::ClipboardTextItem(QClipboard *clipboard)
{
  _type=ClipboardMimeType::Text;
  if(clipboard && clipboard->mimeData()->hasText())
    this->text(clipboard->text());
  else
    _text=new QString("");
}

ClipboardTextItem::ClipboardTextItem(const QString &text)
{
  _type=ClipboardMimeType::Text;
  this->text(text);
}

ClipboardTextItem::ClipboardTextItem(QString *text)
{
  _type=ClipboardMimeType::Text;
  this->text(*text);
}

ClipboardTextItem::~ClipboardTextItem()
{
  deleteTextFile();
  if(_text)
    delete _text;
}

void ClipboardTextItem::text(const QString &text)
{

  if(text.length()>GSettings::inMemoryTextLength)
  {

    QFile *file=new QFile(QString(Resources::tempFolder.path()+"/%1TEXT.genius").arg(_ref));
    file->open(QFile::WriteOnly);
    qint64 bytwrtn=file->write(text.toUtf8());
    file->close();
    if(bytwrtn<0)
    {
      qDebug()<<"cannot write new text to file ClipboardTextItem::text(QString text)";
      if(file->exists())
        file->remove();
      delete file;
      _constructed=false;
      return;
    }
    else
    {
      if(!(_textFile && _textFile->fileName()==file->fileName()))
      {
        deleteTextFile();
        _textFile=file;
      }
      else
        _length=text.length();
    }

    if(_text)
      delete _text;
    _text=new QString(text.left(GSettings::inMemoryTextLength));
  }
  else
  {
    deleteTextFile();
    _length=text.length();
    if(_text)
      delete _text;
    _text=new QString(text);
  }
}

QString *ClipboardTextItem::text()
{
  if(_textFile)
  {
    if(_textFile->exists())
    {
      _textFile->open(QFile::ReadOnly);
      QString *text= new QString (QString::fromUtf8(_textFile->readAll()));
      _textFile->close();
      return text;
    }
    return new QString("");
  }
  else
  {
    if(_text)
      return new QString(*_text);
    else
      return new QString;
  }
}

QString *ClipboardTextItem::preview()
{
  if(_text)
    return _text;
  else
    return NULL;
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

int ClipboardTextItem::length()
{
  return _length;
}
