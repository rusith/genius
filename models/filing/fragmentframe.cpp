#include "fragmentframe.h"

FragmentFrame::FragmentFrame()
{
  size=0;
  start=0;
}

void FragmentFrame::operator =(const FragmentFrame &other)
{
  if(&other==this) return;
  size=other.size;
  start=other.start;
}

bool FragmentFrame::operator ==(const FragmentFrame &other) const
{
  if(this==&other)return true;
  return (other.size==size && other.start==start);
}

bool FragmentFrame::operator <(const FragmentFrame &other)const
{
  if(this == &other)
    return false;
  return size<other.size;
}

bool FragmentFrame::operator >(const FragmentFrame &other) const
{
  if(this==&other)
    return false;
  return size>other.size;
}

bool FragmentFrame::operator <(const quint64 &other) const
{
  return size<other;
}

bool FragmentFrame::operator >(const quint64 &other) const
{
  return size>other;
}

