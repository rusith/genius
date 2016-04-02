#include "clipboardhistory.h"

ClipboardHistory::ClipboardHistory(QObject *parent) : QObject(parent)
{
  _list=new QList<ClipboardEntity*>();
}

ClipboardHistory::~ClipboardHistory()
{
  if(_list)
  {
    if(_list->isEmpty())
      delete _list;
    else
    {
      foreach (ClipboardEntity *entity, *_list)
      {
        if(entity)
        {
          delete entity;
        }
      }
      _list->clear();
      delete _list;
    }
  }
}

void ClipboardHistory::append(ClipboardEntity *entity)
{
  if(entity && _list->length()<=GSettings::maximumItemsInHistory)
  {
    _list->append(entity);
    emit added(entity,_list->length()-1);
  }
}

void ClipboardHistory::pushFront(ClipboardEntity *entity)
{
  if(entity)
  {
    manageLength();
    _list->push_front(entity);
    emit added(entity,0);
  }
}

void ClipboardHistory::remove(const int &reference)
{
  int length=this->length();
  if(length<1)
    return;
  ClipboardEntity *entity;
  int refer;
  for(int i=0;i<length;i++)
  {
   entity=_list->at(i);
   if(entity)
   {
     refer=entity->ref();
     if(refer==reference)
     {
       delete _list->takeAt(i);
       emit removed(reference,i);
       return;
     }
   }
  }
}

void ClipboardHistory::remove(ClipboardEntity *entity)
{
  if(entity)
  {
    if(_list->contains(entity))
    {
      int index=_list->indexOf(entity);
      _list->removeAt(index);
      int ref=entity->ref();
      delete entity;
      removed(ref,index);
      return;
    }
    delete entity;
  }
}

void ClipboardHistory::removeAt(int index)
{
  int length=this->length();
  if(length<1)
    return;
  ClipboardEntity *entity=_list->takeAt(index);
  if(entity)
  {
    delete entity;
    emit removed(entity->ref(),index);
  }
}

void ClipboardHistory::clear()
{
  if(!isEmpty())
  {
    foreach (ClipboardEntity *entity, *_list)
    {
      if(entity)
      {
        delete entity;
      }
    }
    _list->clear();
    emit cleared();
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

ClipboardEntity *ClipboardHistory::get(const int &reference)
{
   if(!isEmpty())
   {
     foreach (ClipboardEntity *entity, *_list)
     {
      if(entity)
      {
        if(entity->ref()==reference)
        {
          return entity;
        }
      }
     }
     return NULL;
   }
   return NULL;
}

ClipboardEntity *ClipboardHistory::at(const int &index)
{
  int length=this->length();
  if(length<1||index<0||index>(length-1))
    return NULL;
  return _list->at(index);
}

ClipboardEntity *ClipboardHistory::first()
{
  if(!isEmpty())
    return _list->first();
  else
    return NULL;
}

void ClipboardHistory::itemUpdated(ClipboardEntity *entity)
{
  if(entity)
    emit updated(entity);
}

int ClipboardHistory::indexOf(const int &reference)
{
  if(_list)
  {
    if(_list->length()>0)
    {
      int len=length();
      ClipboardEntity *entity;
      for(int i=0;i<len;i++)
      {
        entity=_list->at(i);
        if(entity)
        {
          if(entity->ref()==reference)
            return i;
        }
      }
      return -1;
    }
    else return -1;
  }
  else
    return -1;
}

int ClipboardHistory::indexOf(ClipboardEntity *entity)
{

  if(entity && _list->isEmpty()==false)
  {
    int index=0;

    foreach (ClipboardEntity *ent, *_list)
    {
      if(ent->ref()==entity->ref())
        return index;
      index++;
    }
    return -1;
  }
  else
    return -1;
}

void ClipboardHistory::manageLength()
{
  if(_list->length()>0)
  {
     int max=GSettings::maximumItemsInHistory;
     while (_list->length()>=max)
       remove(_list->last());

  }
}

void ClipboardHistory::exchangeLocation(int ref1, int ref2)
{
  int index1=indexOf(ref1);
  int index2=indexOf(ref2);
  if(index1!=-1 && index2!=-1)
  {
    _list->swap(index1,index2);
    emit locationExchanged(ref1,ref2);
  }
}
