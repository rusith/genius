#include "clipboardhistory.h"

ClipboardHistory::ClipboardHistory(QObject *parent) : QObject(parent)
{
  _list=new QList<ClipboardItem*>();
}

ClipboardHistory::~ClipboardHistory()
{
  if(!_list->isEmpty())
  {
    int length=_list->length();
    ClipboardItem *item;
    for(int i=0;i<length;i++)
    {
      item=_list->at(i);
      if(item)
      {
        delete item;
      }
    }
  }
  delete _list;
}



void ClipboardHistory::append(ClipboardItem *item)
{
  if(item && _list->length()<=GSettings::maximumItemsInHistory)
  {
    _list->append(item);
    emit added(item,_list->length()-1);
  }
}

void ClipboardHistory::pushFront(ClipboardItem *item)
{
  if(item)
  {
    _list->push_front(item);
    manageLength();
    emit added(item,0);
  }
}

bool ClipboardHistory::isEmpty()
{
  return _list->isEmpty();
}

int ClipboardHistory::length()
{
  return _list->length();
}

ClipboardItem *ClipboardHistory::get(int &reference)
{
   if(isEmpty()==false)
   {
     int length=this->length();
     ClipboardItem *item;
     for(int i=0;i<length;i++)
     {
       item=_list->at(i);
       if(item && item->ref()==reference)
       {
         return item;
       }
     }
     return NULL;
   }
   return NULL;
}

ClipboardItem *ClipboardHistory::at(int index)
{
  int length=this->length();
  if(length<1||index<0||index>(length-1))
    return NULL;
  return _list->at(index);
}

void ClipboardHistory::remove(int reference)
{
  int length=this->length();
  if(length<1)
    return;
  ClipboardItem *item;
  int refer;
  for(int i=0;i<length;i++)
  {

    item=_list->at(i);
    if(item)
    {
      refer=item->ref();
      if(refer==reference)
      {
        delete _list->takeAt(i);
        emit removed(reference,i);
        return;
      }
    }
  }
}

ClipboardItem *ClipboardHistory::first()
{
  if(!isEmpty())
  {
    return _list->first();
  }
  else
  {
    return NULL;
  }
}

void ClipboardHistory::removeAt(int index)
{
  int length=this->length();
  if(length<1)
    return;
  ClipboardItem *item=_list->takeAt(index);
  if(item)
  {
    emit removed(item->ref(),index);
    delete item;
  }

}

void ClipboardHistory::itemUpdated(ClipboardItem *item)
{
  if(item)
    emit updated(item);
}

void ClipboardHistory::manageLength()
{
  int max=GSettings::maximumItemsInHistory;
  while (_list->length()>max)
  {
    removeAt(_list->length()-1);
  }
}

void ClipboardHistory::clear()
{
  if(!isEmpty())
  {
    int length=this->length();
    ClipboardItem *item;
    for(int i=0;i<length;i++)
    {
      item=_list->at(i);
      if(item)
      {
        delete item;
      }
    }
    _list->clear();
    emit cleared();
  }
}
