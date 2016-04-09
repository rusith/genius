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
  updatePositionLabel();
}

void Selector::initializeUI()
{
//  QRect r=frameGeometry();
//  r.moveCenter(availableGeometry());
//  move(r.topLeft());
//  adjustSize()

  raise();
 // QApplication::setActiveWindow(this);
  move(QApplication::desktop()->screen()->rect().center()-rect().center());

//  for(int i=0;i<10;i++)
//  {
//    if(isActiveWindow()==false)
//      QApplication::setActiveWindow(this);
//    else
//      break;
//  }
  int length=_history->length();
  if(length>1)
  {
    labels.append(createLabel(previousG,_history->at(0)));
    labels.append(createLabel(currentG,_history->at(1)));
    _currentIndex=1;
    if(length>2)
      labels.append(createLabel(nextG,_history->at(2)));
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

  ui->lblPosition->setStyleSheet(QString("color: %1;").arg(GSettings::selectorBorderColor));
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

QLabel *Selector::createLabel(QRect &geometry, ClipboardEntity *content)
{
  QLabel *newLabel=new QLabel(this);
  newLabel->setGeometry(geometry);
  newLabel->setWordWrap(true);
  newLabel->setScaledContents(true);
  newLabel->setAlignment(Qt::AlignCenter);
  QString style=QString("QLabel { color:"+GSettings::selectorTextColor+"; background:"+GSettings::selectorItemBackgroundColor+"; background-repeat: repeat-y; background-position: left; border:%1px solid "+GSettings::selectorBorderColor+"; border-radius: "+QString("%1").arg(GSettings::selectorBorderRadius)+"px;};").arg(GSettings::selectorBorderSize);
  newLabel->setStyleSheet(style);
  QGraphicsOpacityEffect *opacityEffect=new QGraphicsOpacityEffect(newLabel);
  opacityEffect->setOpacity((geometry==currentG?1:0.5));
  newLabel->setGraphicsEffect(opacityEffect);
  if(content)
    constructLabel(newLabel,content);
  newLabel->show();
  return newLabel;
}

void Selector::constructLabel(QLabel *label, ClipboardEntity *entity)
{
  if(label && entity)
  {
    if(entity->hasImage())
    {
      QImage *img=entity->image(false,GSettings::maximumImageWidth,GSettings::maximumImageWidth);
      if(img)
      {
        label->setPixmap(QPixmap::fromImage(*img));
        delete img;
      }
    }
    else if(entity->hasURLs())
    {
      QList<QUrl> urls=entity->urls();
      label->setText(ToolKit::URLsToPreviewText(&urls,100));
    }
    else if(entity->hasHTML())
    {
      label->setText(entity->HTMLText(false,-1));
    }
    else if(entity->hasPlainText())
    {
      label->setText(entity->plainText(false,50));
    }
    else
    {
      QString text=QString("formats : %1 size : %2 KB")
                   .arg(entity->formats().size())
                   .arg((double)entity->size()/1024);
      label->setText(text);
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

  int duration=GSettings::selectorAnimationDuration;
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
  int duration=GSettings::selectorAnimationDuration;
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
    updatePositionLabel();

  }
  else if(key==Qt::Key_Right)
  {
    gotoPrevious();
        updatePositionLabel();

  }
  else if(key==Qt::Key_Escape)
  {
    hide();
  }
}


void Selector::keyReleaseEvent(QKeyEvent *event)
{
  int key=event->key();
  if(key==Qt::Key_Control||key==Qt::Key_Shift)
    hide();
}

void Selector::hideEvent(QHideEvent *event)
{
  emit closing(_currentIndex);
  event->accept();
}

void Selector::showEvent(QShowEvent *event)
{
  if(event)
  {
   initialize();
   event->ignore();
  }
}

void Selector::updatePositionLabel()
{
  ui->lblPosition->setText(QString("<b>%1/%2</b>").arg(_currentIndex).arg(_history->length()));
}


void Selector::changeEvent(QEvent *event)
{
  QWidget::changeEvent(event);
  if(event->type()==QEvent::ActivationChange)
  {
    if(isActiveWindow()==false)
      hide();
  }

}



