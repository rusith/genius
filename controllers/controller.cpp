#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
  _history=new ClipboardHistory();
  _clipboard=QApplication::clipboard();
  _managerOpened=false;

}

Controller::~Controller()
{
  if(_manager)
    delete _manager;

  if(_history)
    delete _history;

  if(_trayIcon)
    delete _trayIcon;

  if(_selector)
    delete _selector;

  if(_openSelectorHotkey)
  {
    _openSelectorHotkey->setRegistered(false);
    delete _openSelectorHotkey;
  }
}

void Controller::start()
{
  GSettings::initialize();
  _manager=new Manager(_history,_clipboard,0);
  _trayIcon=new TrayIcon(_history);
  _selector=new Selector(_history);
  _openSelectorHotkey=new QHotkey(QKeySequence(tr("Ctrl+Shift+V")),false,this);
  _openSelectorHotkey->setRegistered(true);
  makeConnections();

  _manager->initialize();
  if(!_managerOpened && !GSettings::openMinimized)
  {
    _manager->show();
  }

  _trayIcon->show();
}

void Controller::makeConnections()
{
  connect(_clipboard,SIGNAL(changed(QClipboard::Mode)),this,SLOT(clipboardChanged(QClipboard::Mode)));
  connect(_history,SIGNAL(added(ClipboardItem*,int)),this,SLOT(history_itemAdded(ClipboardItem*,int)));
  connect(_history,SIGNAL(removed(int,int)),this,SLOT(history_removed(int,int)));
  connect(_history,SIGNAL(cleared()),this,SLOT(history_cleared()));
  connect(_history,SIGNAL(updated(ClipboardItem*)),this,SLOT(history_itemUpdated(ClipboardItem*)));
  connect(_manager,SIGNAL(shown()),this,SLOT(manager_shown()));
  connect(_manager,SIGNAL(hidden()),this,SLOT(manager_hidden()));
  connect(_manager,SIGNAL(hidden()),_trayIcon,SLOT(managerHidden()));
  connect(_manager,SIGNAL(shown()),_trayIcon,SLOT(managerShown()));
  connect(_trayIcon,SIGNAL(showHideManagerTriggerd()),this,SLOT(showHideManagerRequest()));
  connect(_manager,SIGNAL(settingsDialogRequested()),this,SLOT(settingsdialogRequested()));
  connect(_trayIcon,SIGNAL(itemSelected(int)),this,SLOT(itemSelected(int)));
  connect(_manager,SIGNAL(itemSelected(int)),this,SLOT(itemSelected(int)));
  connect(_manager,SIGNAL(editRequested(ClipboardItem*)),this,SLOT(editRequested(ClipboardItem*)));
  connect(_openSelectorHotkey,SIGNAL(activated()),this,SLOT(openSelectorHKtriggered()));
  connect(_selector,SIGNAL(closing(int)),this,SLOT(selectorClosed(int)));
}

bool Controller::sameDataAgain()
{
  if(!_history||_history->isEmpty())return false;
  ClipboardItem *item=_history->first();
  ClipboardItem::ClipboardMimeType type=item->type();
  if(_clipboard->mimeData()->hasText() && type==ClipboardItem::Text){
      return *item->text()==_clipboard->text();
  }
  else if(_clipboard->mimeData()->hasImage() && type==ClipboardItem::Image){
      return *item->image()==_clipboard->image();
  }
  else if(_clipboard->mimeData()->hasUrls() && type==ClipboardItem::URLs){
      return *item->urls()==_clipboard->mimeData()->urls();
  }
  else return false;
}

void Controller::clipboardChanged(QClipboard::Mode mode)
{
  if(mode==QClipboard::Clipboard)
  {
    if(!sameDataAgain())
    {
      ClipboardItem *item=new ClipboardItem(_clipboard);
      ClipboardItem::ClipboardMimeType type=item->type();
      if((type==ClipboardItem::Image && GSettings::saveImages)||(type==ClipboardItem::URLs && GSettings::saveUrls)||type==ClipboardItem::Text )
      {
        _history->pushFront(item);
      }
    }
  }
}

void Controller::manager_hidden()
{
  _managerOpened=false;
}

void Controller::manager_shown()
{
  _managerOpened=true;
}

void Controller::history_itemAdded(ClipboardItem *item, int index)
{
  if(!item)return;
  int reference=item->ref();
  ClipboardItem::ClipboardMimeType type=item->type();
  if(type==ClipboardItem::Text)
  {
    QString *text=new QString(*item->text());
    if(GSettings::showInSingleLine)
      ToolKit::removeNewLines(text);
    if(GSettings::limitcharLength)
    {
      if(text->length()>GSettings::limitedCharLength)
        *text=text->left(GSettings::limitedCharLength)+"...";
    }

    QString tooltipText="type : text"+QString("\ncontent length : %1").arg(item->text()->length())+QString("\nadded time : %1 ").arg(item->addedTime()->toString("hh.mm.ss.zzz AP"));
    _manager->addTextItem(text,&tooltipText,reference,index);
    _trayIcon->addTextAction(text,&tooltipText,reference,index);
    delete text;
  }
  else if(type==ClipboardItem::Image)
  {

    QImage *image=item->image();
    QIcon icon(QPixmap::fromImage(*image));
    QString text(QString("width : %1").arg(image->width()));
    text+=QString("  height : %1").arg(image->height());
    text+="  added time : "+item->addedTime()->toString("hh.mm.ss.zzz AP");

    _manager->addImageItem(&text,&icon,reference,index);
    _trayIcon->addImageAction(&text,&icon,reference,index);
  }
  else if(type==ClipboardItem::URLs)
  {
    QString *text=new QString(ToolKit::URlsToString(item->urls()));
    if(GSettings::showInSingleLine)
      ToolKit::removeNewLines(text);
    if(GSettings::limitcharLength)
    {
      if(text->length()>GSettings::limitedCharLength)
        *text=text->left(GSettings::limitedCharLength)+"...";
    }

    QString tooltipText="type : urls"+QString("\nurls : %1").arg(item->urls()->length())+QString("\nadded time : %1 ").arg(item->addedTime()->toString("hh.mm.ss.zzz AP"));
    _manager->addTextItem(text,&tooltipText,reference,index);
    _trayIcon->addTextAction(text,&tooltipText,reference,index);
    delete text;
  }
}

void Controller::settingsdialogRequested()
{
  Settings settings;
  if(settings.exec())
  {
    GSettings::commit();
  }
}

void Controller::history_removed(int reference, int index)
{
  if(index>-1)
  {
    _manager->removeItem(reference);
    _trayIcon->removeItem(reference);
  }
}

void Controller::history_cleared()
{
  _manager->clearList();
  _trayIcon->clearHistoryList();
}

void Controller::history_itemUpdated(ClipboardItem *item)
{
  if(item)
  {
    ClipboardItem::ClipboardMimeType type=item->type();
    if(type==ClipboardItem::Text)
    {
      QString *text=new QString(*item->text());
      if(GSettings::showInSingleLine)
        ToolKit::removeNewLines(text);
      if(GSettings::limitcharLength)
      {
        if(text->length()>GSettings::limitedCharLength)
          *text=text->left(GSettings::limitedCharLength)+"...";
      }
      int reference=item->ref();
      QString tooltipText="type : text"+QString("\ncontent length : %1").arg(item->text()->length())+QString("\nadded time : %1 ").arg(item->addedTime()->toString("hh.mm.ss.zzz AP"));
      _manager->updateTextItem(text,&tooltipText,reference);
      _trayIcon->updateTextItem(text,&tooltipText,reference);
      delete text;
    }
    else if(type==ClipboardItem::Image)
    {

      QImage *image=item->image();
      QIcon icon(QPixmap::fromImage(*image));
      QString text(QString("width : %1").arg(image->width()));
      text+=QString("  height : %1").arg(image->height());
      text+="  added time : "+item->addedTime()->toString("hh.mm.ss.zzz AP");
      int ref=item->ref();
      _manager->updateImageItem(&text,&icon,ref);
      _trayIcon->updateImageItem(&text,&icon,ref);
    }
    else if(type==ClipboardItem::URLs)
    {
      QString *text=new QString(ToolKit::URlsToString(item->urls()));
      if(GSettings::showInSingleLine)
        ToolKit::removeNewLines(text);
      if(GSettings::limitcharLength)
      {
        if(text->length()>GSettings::limitedCharLength)
          *text=text->left(GSettings::limitedCharLength)+"...";
      }

      QString tooltipText="type : urls"+QString("\nurls : %1").arg(item->urls()->length())+QString("\nadded time : %1 ").arg(item->addedTime()->toString("hh.mm.ss.zzz AP"));
      int ref=item->ref();
      _manager->updateTextItem(text,&tooltipText,ref);
      _trayIcon->updateTextItem(text,&tooltipText,ref);
      delete text;
    }
  }
}

void Controller::showHideManagerRequest()
{
  if(_managerOpened)
  {
    _manager->hide();
  }
  else
  {
    _manager->show();
  }
}

void Controller::itemSelected(int reference)
{
  if(_history->isEmpty()==false)
  {
    ClipboardItem *item=_history->get(reference);
    if(item)
    {
      ClipboardItem::ClipboardMimeType type=item->type();
      if(type==ClipboardItem::Text)
      {
        QString str=*item->text();
        _history->remove(reference);
        _clipboard->setText(str);
      }
      else if(type==ClipboardItem::Image)
      {
        QImage img=*item->image();
        _history->remove(reference);
        _clipboard->setImage(img);
      }
      else if(type==ClipboardItem::URLs)
      {
        QMimeData *mimedata=new QMimeData();
        mimedata->setUrls(*item->urls());
        _history->remove(reference);
        _clipboard->setMimeData(mimedata);
      }

    }
  }
}

void Controller::editRequested(ClipboardItem *item)
{
  if(item)
  {
    ClipboardItem::ClipboardMimeType type=item->type();
    if(type==ClipboardItem::Text)
    {
      QString temp(*item->text());
      TextEditor textEditor(item->text());
      bool accept=textEditor.exec();
      if(accept)
      {
        if(temp!=*item->text())
          _history->itemUpdated(item);
      }
    }
    else if(type==ClipboardItem::Image)
    {
      QImage temp(*item->image());
      ImageEditor IE(item->image());
      bool accept=IE.exec();
      if(accept)
      {
        if(temp!=*item->image())
          _history->itemUpdated(item);
      }
    }
  }
}

void Controller::selectorClosed(int currentIndex)
{

  ClipboardItem *item=_history->at(currentIndex);
  if(item)
  {
    int reference=item->ref();
    itemSelected(reference);
    if(GSettings::pasteAutomaticlay)
    {
      FakeKey::simulatePaste();
    }
  }

  _selectorOpen=false;
}

void Controller::openSelectorHKtriggered()
{

  if(!_selectorOpen && _history->isEmpty()==false)
  {
    _selector->show();
    _selectorOpen=true;
  }
}
