#include "menuitem.h"

MenuItem::MenuItem(QObject *parent) : QAction(parent)
{
  indexed_=false;
  indexLength_=0;
  index_=0;
}
void MenuItem::index(const int &inde)
{
  if(indexed_)
  {
    index_=inde;
    QString indexString=QString("%1. ").arg(index_);
    QString otext(text());
    QString txt=otext.mid(indexLength_,otext.length());
    setText(indexString+txt);
  }
}

void MenuItem::indexed(bool inde)
{
  /*if(inde==false)
    if(indexed)
      remove index text , set text
  else
    if(!indexed)
      get text - append index Text
      set text
*/

  if(!inde)
  {
    if(indexed_)
    {
      if(indexLength_>0)
      {
        QString current(text());
        QString original(current.mid(indexLength_,current.length()));
        setText(original);
        indexLength_=0;
      }
      indexed_=false;
    }
  }
  else if(!indexed_)
  {
    QString current(text());
    index_=0;
    QString indexText(QString("%1. ").arg(index_));
    setText(indexText+current);
    indexLength_=indexText.length();
    indexed_=true;
  }




//  if(inde)
//  {
//    if(!indexed_)
//    {
//      index_=1;
//      QString indexString=QString("%1. ").arg(index_);
//      indexLength_=indexString.length();
//      QString txt=text();
//      setText(indexString+txt);
//      indexed_=true;
//    }
//    else
//    {
//      setTxt(text());
//    }
//  }
//  else
//  {
//    if(indexed_)
//    {
//      indexed_=false;
//      setText(text());
//    }
//    else
//    {
//      setText(text());
//    }
//  }

}

void MenuItem::setTxt(const QString &text)
{
  //if indexed , append index string and set text
  //else - just set the Text
  if(indexed_)
  {
    QString indexString=QString("%1. ").arg(index_);
    indexLength_=indexString.length();
    setText(indexString+text);
  }
  else
    setText(text);
}

const int &MenuItem::index()
{
  return index_;
}

bool MenuItem::indexed()
{
  return indexed_;
}
