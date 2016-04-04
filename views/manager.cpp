#include "manager.h"
#include "ui_manager.h"

Manager::Manager(ClipboardHistory *history, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::Manager)
{
  ui->setupUi(this);
  if(history)
    _history=history;
}

Manager::~Manager()
{
  delete ui;
}

void Manager::addTextItem(QString *text, QString *tooltipText, int reference, int index)
{

  QListWidgetItem *qlwi=new QListWidgetItem();
  qlwi->setText(*text);  
  qlwi->setToolTip(*tooltipText);
  qlwi->setData(50,reference);
  ui->list->insertItem(index,qlwi);
}

void Manager::addImageItem(QString *text, QIcon *icon,int reference,int index)
{
  QListWidgetItem *qlwi=new QListWidgetItem();
  qlwi->setIcon(*icon);
  qlwi->setText(*text);
  qlwi->setData(50,reference);
  ui->list->insertItem(index,qlwi);
}

void Manager::removeItem(int reference)
{
   int count=ui->list->count();
  if(count>0)
  {
    QListWidgetItem *item;
    for(int i=0;i<count;i++)
    {
      item=ui->list->item(i);
      if(item)
      {
        int ref=item->data(50).toInt();
        if(ref==reference)
        {
          delete item;
          return;
        }
      }
    }
  }
}

void Manager::clearList()
{
  int length=ui->list->count();
  QListWidgetItem *item;
  for(int i=0;i<length;i++)
  {
    item=ui->list->item(i);
    if(item)
    {
      delete item;
    }
  }
  ui->list->clear();
}

void Manager::initialize()
{
  initializeUI();
}

void Manager::initializeUI()
{
  ui->centralwidget->setLayout(ui->rootLayout);
  setCentralWidget(ui->centralwidget);
}


void Manager::on_deleteButton_clicked()
{
  int currentRow=ui->list->currentRow();
  if(_history->isEmpty()==false && currentRow>-1)
  {
    QListWidgetItem *item=ui->list->item(currentRow);
    if(item)
    {
      _history->remove(item->data(50).toInt());
    }
  }
}

void Manager::on_settingsButton_clicked()
{
    emit settingsDialogRequested();
}

void Manager::on_clearButton_clicked()
{
  if(_history && !_history->isEmpty())
  {
    _history->clear();
  }
}

void Manager::on_list_itemDoubleClicked(QListWidgetItem *item)
{
  if(item)
  {
    int ref=item->data(50).toInt();
    emit itemSelected(ref);
  }
}

void Manager::on_list_customContextMenuRequested(const QPoint &pos)
{

  QListWidgetItem* item=ui->list->itemAt(pos);
  if(!item)
     return;
   int reference=item->data(50).toInt();
   ClipboardEntity* cpEnt=_history->get(reference);
   int index=ui->list->row(item);
   if(cpEnt)
   {
     QMenu contextMenu(this);

     QAction select(&contextMenu);
     select.setIconText("select");
     select.setIcon(QIcon(Resources::ok16));
     select.setData("Select");
     contextMenu.addAction(&select);

     QAction showContent(&contextMenu);
     showContent.setIcon(QIcon(Resources::show16));
     showContent.setData("ShowContent");
     showContent.setIconText("show content");
     contextMenu.addAction(&showContent);

     QAction moveUp(&contextMenu);
     QAction moveDown(&contextMenu);
     if(ui->list->count()>1)
     {
       if(index>1)
       {
         moveUp.setIcon(QIcon(Resources::up16));
         moveUp.setIconText("move up");
         moveUp.setData("MoveUp");
         contextMenu.addAction(&moveUp);
       }
       if(index<ui->list->count()-1 && index>0)
       {
         moveDown.setIcon(QIcon(Resources::downn16));
         moveDown.setIconText("move down");
         moveDown.setData("MoveDown");
         contextMenu.addAction(&moveDown);
       }
     }



     QAction del(&contextMenu);
     del.setIcon(QIcon(Resources::dustBin16));
     del.setIconText("delete");
     del.setData("Delete");
     contextMenu.addAction(&del);



     connect(&contextMenu,SIGNAL(triggered(QAction*)),this,SLOT(itemContextMenuTriggered(QAction*)));
     contextMenu.exec(ui->list->mapToGlobal(pos));
   }
}

void Manager::itemContextMenuTriggered(QAction *action)
{
  int length=ui->list->count();
  if(length>0)
  {
    int selected=ui->list->currentRow();
    if(selected>-1)
    {
      QListWidgetItem *qlwi=ui->list->item(selected);
      if(qlwi)
      {
        int reference=qlwi->data(50).toInt();
        ClipboardEntity *entity=_history->get(reference);
        if(entity)
        {
          QString com=action->data().toString();
          if(com=="ShowContent")
            emit showContentRequested(entity);
          else if(com=="Delete")
            _history->remove(reference);
          else if(com=="Select")
            emit itemSelected(reference);
          else if(com=="MoveUp")
          {
            if(_history->length()>1 && selected>1)
            {
              int nextRef=ui->list->item(selected-1)->data(50).toInt();
              emit locationExchangeRequested(nextRef,reference);
              //emit entityMoveUpRequested(entity);
            }
          }
          else if(com=="MoveDown")
          {
            if(_history->length()>1 && selected!=0 && selected<ui->list->count()-1)
            {
              int nextRef=ui->list->item(selected+1)->data(50).toInt();
              emit locationExchangeRequested(nextRef,reference);
              //emit entityMoveDownRequested(entity);
            }
          }
          else return;
        }
      }
    }
  }

}

void Manager::on_showContentButton_clicked()
{
  int length=ui->list->count();
  if(length>0)
  {
    int currentIndex=ui->list->currentRow();
    if(currentIndex>-1)
    {
      QListWidgetItem *item=ui->list->item(currentIndex);
      if(item)
      {
        int ref=item->data(50).toInt();
        ClipboardEntity *Clenty=_history->get(ref);
        if(Clenty)
        {
          emit showContentRequested(Clenty);
          return;
        }
      }
    }
  }
}

void Manager::showEvent(QShowEvent *event)
{
    if (event)
    {
      event->accept();
      emit shown();
    }
}

void Manager::hideEvent(QHideEvent *event)
{
  if(event)
  {
    event->accept();
    emit hidden();
  }
}

void Manager::closeEvent(QCloseEvent *event)
{
  event->ignore();
  hide();
}

void Manager::exchangeLocations(int ref1, int ref2)
{
  QListWidgetItem *item1;
  QListWidgetItem *item2;
  QListWidgetItem *item;
  int item1Index=0;
  int item2Index=0;
  for(int i=0;i<ui->list->count();i++)
  {
    item=ui->list->item(i);
    if(item)
    {
      int data=ui->list->item(i)->data(50).toInt();
      if(data==ref1)
      {
        item1=item;
        item1Index=i;
      }
      else if(data==ref2)
      {
        item2=item;
        item2Index=i;
      }
    }
  }
  QListWidgetItem *temp=new QListWidgetItem(*item1);
  *item1=*item2;
  *item2=*temp;
  delete temp;
  int row=ui->list->currentRow();
  if(row==item1Index)
    ui->list->setCurrentRow(item2Index);
  else if(row==item2Index)
    ui->list->setCurrentRow(item1Index);
}

//void Manager::updateInfoLabel(const int &currentRow)
//{
//  if(ui->list->count()<1)
//    ui->infoLabel->hide();

//  if(currentRow<0)
//    ui->infoLabel->hide();
//  else
//  {
//    int ref=ui->list->item(currentRow)->data(50).toInt();
//    ClipboardEntity *ent=_history->get(ref);
//    if(ent)
//    {
//      ui->infoLabel->setText(QString("<b>formats :</b> %1   <b>size :</b> %2 kb").arg(ent->formats().count()).arg((double)ent->size()/1024));
//      if(ui->infoLabel->isHidden())
//        ui->infoLabel->show();
//    }
//  }
//}

//void Manager::on_list_currentRowChanged(int currentRow)
//{
    //updateInfoLabel(currentRow);
//}
