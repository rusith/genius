#ifndef SELECTOR_H
#define SELECTOR_H

#include <QMainWindow>
#include <QRect>
#include <QHotkey>
#include <QLabel>
#include <QList>
#include <QPixmap>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QKeyEvent>
#include <QHideEvent>
#include <QDebug>
#include <models/clipboardtextitem.h>
#include <models/clipboardimageitem.h>
#include <models/clipboardurlitem.h>
#include <models/clipboardhistory.h>


namespace Ui {
class Selector;
}

class Selector : public QMainWindow
{
  Q_OBJECT

public:
  explicit Selector(ClipboardHistory *history, QWidget *parent = 0);
  ~Selector();
  int currentIndex();
signals:
  void closing(int CI);
private slots:
  void animationCompleted();
private:
  Ui::Selector *ui;
  ClipboardHistory *_history;
  void initialize();
  void initializeUI();
  void initializeVarbs();
  void constructLabel(QLabel *label, ClipboardEntity *entity);
  void gotoNext();
  void gotoPrevious();
  QLabel *getLabel(int what);
  void deleteLabel(QLabel *label);


  QLabel *createLabel(QRect &geometry, ClipboardEntity *content);
  QHotkey nextHotkey;
  QHotkey previousHotkey;

  QList<QLabel*> labels;

  QRect nextG;
  QRect previousG;
  QRect currentG;
  QRect OutPG;
  QRect OutNG;

  int _currentIndex;

  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);
  void showEvent(QShowEvent *event);
  void hideEvent(QHideEvent *event);
};

#endif // SELECTOR_H
