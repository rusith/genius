#include "selector.h"
#include "ui_selector.h"

Selector::Selector(ClipboardHistory *history, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::Selector)
{
  ui->setupUi(this);
  if(history)
  {
    _history=history;
  }

  setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
  setAttribute(Qt::WA_TranslucentBackground,true);

}

Selector::~Selector()
{
  delete ui;
}

void Selector::initialize()
{
  initializeVarbs();
  initializeUI();
}

void Selector::initializeUI()
{
  QApplication::setActiveWindow(this);
  int length=_history->length();
  if(length>1)
  {
    labels.append(createLabel(previousG,_history->at(0)));
    labels.append(createLabel(currentG,_history->at(1)));
    _currentIndex=1;
    if(length>2)
    {
      labels.append(createLabel(nextG,_history->at(2)));
    }
  }
  else if(length==1)
  {
    labels.append(createLabel(currentG,_history->at(0)));
    _currentIndex=0;
  }
  else
  {
    return;
  }
}

void Selector::initializeVarbs()
{
  nextG=QRect(380,60,100,100);
  previousG=QRect(0,60,100,100);
  currentG=QRect(140,0,200,200);
  OutPG=QRect(-100,60,100,100);
  OutNG=QRect(480,60,100,100);

  int length=labels.length();
  if(length>0)
  {
    for(int i=0;i<length;i++)
    {
      QLabel *label=labels.at(i);
      if(label)
      {
        label->hide();
        delete label;
      }
    }
  }
  labels.clear();

  labels=QList<QLabel*>();

}

QLabel *Selector::createLabel(QRect &geometry, ClipboardItem *content)
{
  QLabel *newLabel=new QLabel(this);
  newLabel->setGeometry(geometry);
  newLabel->setWordWrap(true);
  newLabel->setScaledContents(true);
  newLabel->setStyleSheet("QLabel {\n    background: lightblue;\n    background-repeat: repeat-y;\n    background-position: left;\n	border-width: 2px;\n    border-style: solid;\n	\n    border-radius: 15px;\n}");
  QGraphicsOpacityEffect *opacityEffect=new QGraphicsOpacityEffect(newLabel);
  opacityEffect->setOpacity((geometry==currentG?1:0.5));
  newLabel->setGraphicsEffect(opacityEffect);
  if(content)
    constructLabel(newLabel,content);
  newLabel->show();
  return newLabel;
}

void Selector::constructLabel(QLabel *label, ClipboardItem *item)
{
  if(label && item)
  {
    ClipboardItem::ClipboardMimeType type=item->type();
    if(type==item->Text)
      label->setText(*item->text());

    else if(type==item->Image)
    {
      QImage *image=item->image();
      if(image)
      {
        label->setPixmap(QPixmap::fromImage(*item->image()));
      }
    }
  }
}

int Selector::currentIndex()
{
  return _currentIndex;
}

void Selector::gotoNext()
{
  QLabel *lNext=getLabel(1);
  QLabel *lCurrent=getLabel(0);
  QLabel *lPrevious=getLabel(-1);

  if(!lCurrent||!lNext)return;

  int duration=250;
  QParallelAnimationGroup *animationGroup=new QParallelAnimationGroup(this);

  QPropertyAnimation *aNextToCurrentG=new QPropertyAnimation(lNext,"geometry",animationGroup);
  aNextToCurrentG->setDuration(duration);
  aNextToCurrentG->setEndValue(currentG);
  animationGroup->addAnimation(aNextToCurrentG);

  QPropertyAnimation *aNextToCurrentO=new QPropertyAnimation(lNext->graphicsEffect(),"opacity",animationGroup);
  aNextToCurrentO->setEndValue(1.0);
  aNextToCurrentO->setDuration(duration);
  animationGroup->addAnimation(aNextToCurrentO);
  _currentIndex++;

  QPropertyAnimation *aCurrentToPreviousG =new QPropertyAnimation(lCurrent,"geometry",animationGroup);
  aCurrentToPreviousG->setEndValue(previousG);
  aCurrentToPreviousG->setDuration(duration);
  animationGroup->addAnimation(aCurrentToPreviousG);

  QPropertyAnimation *aCurrentToPreviousO=new QPropertyAnimation(lCurrent->graphicsEffect(),"opacity",animationGroup);
  aCurrentToPreviousO->setEndValue(.5);
  aCurrentToPreviousO->setDuration(duration);
  animationGroup->addAnimation(aCurrentToPreviousO);

  if(lPrevious)
  {
   QPropertyAnimation *aPreviousToOutG =new QPropertyAnimation(lPrevious,"geometry",animationGroup);
   aPreviousToOutG->setEndValue(OutPG);
   aPreviousToOutG->setDuration(duration);
   animationGroup->addAnimation(aPreviousToOutG);
  }

  if(_currentIndex<(_history->length()-1))
  {
   QLabel *label=createLabel(OutNG,_history->at(_currentIndex+1));
   labels.append(label);
   QPropertyAnimation *aNoutToNextAnimation=new QPropertyAnimation(label,"geometry",animationGroup);
   aNoutToNextAnimation->setEndValue(nextG);
   aNoutToNextAnimation->setDuration(duration);
   animationGroup->addAnimation(aNoutToNextAnimation);
  }

  connect(animationGroup,SIGNAL(finished()),this,SLOT(animationCompleted()));
  animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
}

void Selector::gotoPrevious()
{
  QLabel *lNext=getLabel(1);
  QLabel *lCurrent=getLabel(0);
  QLabel *lPrevious=getLabel(-1);

  if(!lCurrent||!lPrevious)return;
  int duration=250;
  QParallelAnimationGroup *animationGroup=new QParallelAnimationGroup(this);


  QPropertyAnimation *aPreviousToCurrentG =new QPropertyAnimation(lPrevious,"geometry",animationGroup);
  aPreviousToCurrentG->setEndValue(currentG);
  aPreviousToCurrentG->setDuration(duration);
  animationGroup->addAnimation(aPreviousToCurrentG);

  QPropertyAnimation *aPreviousToCurrentO=new QPropertyAnimation(lPrevious->graphicsEffect(),"opacity",animationGroup);
  aPreviousToCurrentO->setEndValue(1.0);
  aPreviousToCurrentO->setDuration(duration);
  animationGroup->addAnimation(aPreviousToCurrentO);
  _currentIndex--;

  QPropertyAnimation *aCurrentToNextG =new QPropertyAnimation(lCurrent,"geometry",animationGroup);
  aCurrentToNextG->setEndValue(nextG);
  aCurrentToNextG->setDuration(duration);
  animationGroup->addAnimation(aCurrentToNextG);

  QPropertyAnimation *aCurrentToNextO=new QPropertyAnimation(lCurrent->graphicsEffect(),"opacity",animationGroup);
  aCurrentToNextO->setEndValue(.5);
  aCurrentToNextO->setDuration(duration);
  animationGroup->addAnimation(aCurrentToNextO);

  if(lNext)
  {
    QPropertyAnimation *aNextToOutG=new QPropertyAnimation(lNext,"geometry",animationGroup);
    aNextToOutG->setDuration(duration);
    aNextToOutG->setEndValue(OutNG);
    animationGroup->addAnimation(aNextToOutG);
  }

  if(_currentIndex>0)
  {
    QLabel *label=createLabel(OutPG,_history->at(_currentIndex-1));
    labels.append(label);
    QPropertyAnimation *aPoutToPreViousAnimation=new QPropertyAnimation(label,"geometry",animationGroup);
    aPoutToPreViousAnimation->setEndValue(previousG);
    aPoutToPreViousAnimation->setDuration(duration);
    animationGroup->addAnimation(aPoutToPreViousAnimation);
  }

  connect(animationGroup,SIGNAL(finished()),this,SLOT(animationCompleted()));
  animationGroup->start(QAbstractAnimation::DeleteWhenStopped);

}

QLabel *Selector::getLabel(int what)
{
  if(labels.isEmpty()||(what>2||what<-2))
    return NULL;
  QLabel *label;
  int length=labels.length();
  if(what==-1)
  {
    for(int i=0;i<length;i++)
    {
       label=labels.at(i);
       if(label)
       {
         if(label->geometry()==previousG)
           return label;
       }
    }
    return NULL;
  }
  else if(what==0)
  {
    for(int i=0;i<length;i++)
    {
       label=labels.at(i);
       if(label)
       {
         if(label->geometry()==currentG)
           return label;
       }
    }
    return NULL;
  }
  else if(what==1)
  {
    for(int i=0;i<length;i++)
    {
       label=labels.at(i);
       if(label)
       {
         if(label->geometry()==nextG)
           return label;
       }
    }
    return NULL;
  }
  else if(what==-2)
  {
    for(int i=0;i<length;i++)
    {
       label=labels.at(i);
       if(label)
       {
         if(label->geometry()==OutPG)
           return label;
       }
    }
    return NULL;
  }
  else if(what==2)
  {
    for(int i=0;i<length;i++)
    {
       label=labels.at(i);
       if(label)
       {
         if(label->geometry()==OutNG)
           return label;
       }
    }
    return NULL;
  }

  else return NULL;
}

void Selector::animationCompleted()
{
  QLabel *outn=getLabel(2);
  if(outn)
    deleteLabel(outn);
  QLabel *outp=getLabel(-2);
  if(outp)
    deleteLabel(outp);
}

void Selector::deleteLabel(QLabel *label)
{
  if(label)
  {
    int index=labels.indexOf(label);
    if(index>-1)
    {
      QLabel *sL=labels.takeAt(index);
      if(sL)
      {
        delete sL;
      }
    }
  }
}

void Selector::keyPressEvent(QKeyEvent *event)
{
  int key=event->key();
  if(key==Qt::Key_Left)
  {
    gotoNext();
  }
  else if(key==Qt::Key_Right)
  {
    gotoPrevious();
  }
  else if(key==Qt::Key_Escape)
  {

    close();
    emit closing(_currentIndex);
  }
}

void Selector::keyReleaseEvent(QKeyEvent *event)
{
  if(event)
  {
    int key=event->key();
    if(key==Qt::Key_Shift||key==Qt::Key_Control)
    {
      hide();
    }
  }
}

void Selector::hideEvent(QHideEvent *event)
{
  if(event)
  {
    emit closing(_currentIndex);
  }
}

void Selector::showEvent(QShowEvent *event)
{
  if(event)
  {
   initialize();
   event->accept();
  }
}
