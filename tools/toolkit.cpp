#include "toolkit.h"

ToolKit::ToolKit()
{

}

void ToolKit::removeNewLines(QString *text)
{
  if(text)
  {
    *text=text->replace("\n","");
  }
}

QString ToolKit::URlsToString(QList<QUrl> *urls, const QString &seperator, bool numbers, const QString &numFormat)
{
  QString text;

  int length=urls->length();
  if(urls && length>0)
  {
    QString format;
    if(numbers)
    {
      if(numFormat.contains("%1"))
        format.append(numFormat);
      else
        format.append("%1. ");
      format.append("%2");
    }
    else
      format.append("%1");

    if(seperator.isEmpty()==false)
      format.append(seperator);
    else
      format.append("|");
    if(numbers)
    {

      for(int i=0;i<length;i++)
      {
        QUrl url=urls->at(i);
        text.append(format.arg(i).arg(url.toString()));
      }
    }
    else
    {
      for(int i=0;i<length;i++)
      {
        QUrl url=urls->at(i);
        text.append(format.arg(url.toString()));
      }
    }
    return text;
  }
  else
    return text;

}

FragmentFrame ToolKit::maxValue(const QList<FragmentFrame> &frames)
{
  if (frames.isEmpty()) return FragmentFrame();
  if(frames.count()<2) return frames.first();
  FragmentFrame max=frames.first();
  foreach (FragmentFrame frame, frames)
  {
    if(frame>max)
      max=frame;
  }
  return max;
}

void ToolKit::Sort(QList<FragmentFrame> *frames)
{
 // RTimer timer("ToolKit::Sort(QList<FragmentFrame> *frames)","sort list of fragment frames by size");
  quint64 size =frames->first().size;
  int times=0;
  int length=frames->length();
  while (true)
  {
    times=0;
    for(int i=0;i<length;i++)
    {
      if(frames->at(i).size>size)
      {
        frames->swap(i,i-1);
        times++;
        size=frames->at(i).size;
      }
    }
    if(times==0) break;
  }
}

QString ToolKit::URLsToPreviewText(QList<QUrl> *urls,const uint &maxLenght)
{
  QString text;
  if(urls)
  {
    if(!urls->isEmpty())
    {
      bool files=true;
      foreach (QUrl url, *urls)
      {
        if(url.scheme()!="file")
        {
          files=false;
          break;
        }
      }
      if(files)
      {
        text=(urls->count()==1 ?QString("file - "): QString ("%1 files - ").arg(urls->count()));
        if(maxLenght==0)
        {
          foreach (QUrl url, *urls)
          {
            text.append(url.fileName()+",");
          }
        }
        else
        {
          foreach (QUrl url, *urls)
          {
            QString fn=url.fileName();
            if(((uint)text.length()+fn.length())>maxLenght)
            {
              text.append(fn+",");
              text.append(" ..");
              return text;
            }
            else
              text.append(fn+",");
          }
        }
        if(text.endsWith(","))
          text=text.left(text.length()-1);
      }
      else
      {
        text=QString ("%1 urls ").arg(urls->count());
        if(maxLenght==0)
        {
          foreach (QUrl url, *urls)
            text.append(url.toString()+",");
        }
        else
        {
          foreach (QUrl url, *urls)
          {
            QString str=url.toString();
            if(((uint)text.length()+str.length())>maxLenght)
            {
              text.append(" ..");
              return text;
            }
            else
              text.append(str+",");
          }
        }
        if(text.endsWith(","))
          text=text.left(text.length()-1);
      }
    }
  }
  return text;
}

