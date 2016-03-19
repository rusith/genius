#ifndef CLIPBOARDHISTORY_H
#define CLIPBOARDHISTORY_H

#include <QObject>
#include <QList>
#include <models/clipboarditem.h>
#include <controllers/gsettings.h>

class ClipboardHistory : public QObject
{
  Q_OBJECT
public:
  explicit ClipboardHistory(QObject *parent = 0);
  ~ClipboardHistory();

  void append(ClipboardItem *item);
  void pushFront(ClipboardItem *item);

  void remove(int reference);
  void removeAt(int index);
  void clear();
  bool isEmpty();
  int length();


  ClipboardItem *get(int &reference);
  ClipboardItem *at(int index);
  ClipboardItem *first();

  void itemUpdated(ClipboardItem *item);


signals:
  void added(ClipboardItem *item,int index);
  void removed(int reference,int index);
  void updated(ClipboardItem *item);
  void cleared();

public slots:

private:
  //private fields
  QList<ClipboardItem*> *_list;
  int *_max;
  void manageLength();
};

#endif // CLIPBOARDHISTORY_H
