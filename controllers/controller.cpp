#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
  _history=new ClipboardHistory();
  _clipboard=QApplication::clipboard();
  _managerOpened=false;
  _settingsWindowOpened=false;
  _selectorOpen=false;

  GSettings::initialize();

}

Controller::~Controller()
{
  Resources::tempFolder.remove();
  deleteVariables();
  deleteHotkeys();
}

//----------------------------------------------------basic SLOTS------------------------------------------

void Controller::itemSelected(int reference)
{
 selectItem(reference);
}

void Controller::editRequested(ClipboardItem *item)
{
  letToEditItem(item);
}


void Controller::clipboardChanged(QClipboard::Mode mode)
{
  if(mode==QClipboard::Clipboard)
  {
    if(!_holtCollection && !isClipboardEmpty() && !sameDataAgain())
      addClipboardContentToHistory();
    else return;
  }
  else return;
}

//--------------------------------------------------------history control SLOTS-----------------------------

void Controller::history_itemAdded(ClipboardItem *item, int index)
{
  addItem(item,index);
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
  updateItem(item);
}

//----------------------------------------------------view control SLOTS--------------------------------------

void Controller::manager_hidden()
{
  _managerOpened=false;
}

void Controller::manager_shown()
{
  _managerOpened=true;
}

void Controller::settingsWindowRequested()
{
  if(!_settingsWindowOpened)
  {
    _settingsWindow->show();
    _settingsWindowOpened=true;
  }
}

void Controller::showHideManagerRequest()
{
  toggleManager();
}

void Controller::selectorClosed(int currentIndex)
{
  _selectorOpen=false;
  ClipboardItem *item=_history->at(currentIndex);
  if(item)
  {
    int reference=item->ref();
    itemSelected(reference);
    if(GSettings::pasteAutomaticlay)
      FakeKey::simulatePaste();
  }
}

void Controller::settingsWindow_hidden()
{
  enableHotkeys(true);
  if(_settingsWindowOpened)
    _settingsWindowOpened=false;
}

void Controller::turnOffRequest()
{
  _holtCollection=true;
}

void Controller::turnOnRequest()
{
  _holtCollection=false;
}

void Controller::exitRequested()
{
  deleteVariables();
  deleteHotkeys();
  Resources::tempFolder.remove();
  exit(0);
}

//-------------------------------------------Hotkey activation controls (Slots)---------------------------------

void Controller::openSelectorHKtriggered()
{

  if(!_selectorOpen && _history->isEmpty()==false)
  {
    _selector->show();
    _selectorOpen=true;
  }
}

void Controller::clearHistoryHKTrigered()
{
  if(_history->isEmpty()==false)
  {
    int length=_history->length();
    _history->clear();
    _trayIcon->showMessage("history cleard",QString("clipboard history is cleard. \n%1 items was deleted").arg(length),QSystemTrayIcon::Information,1000);
  }
}

void Controller::pasteLasteHKTrigered()
{
  int length=_history->length();
  if(length>1)
  {
    ClipboardItem *item=_history->at(1);
    if(item)
    {
      itemSelected(item->ref());
      FakeKey::simulatePaste();
    }
  }
}

void Controller::openManagerHKTriggered()
{
  if(!_managerOpened)
  {
    _manager->show();
  }
}

void Controller::openSettingsHKTriggered()
{
  if(!_settingsWindowOpened)
  {
    enableHotkeys(false);
    _settingsWindow->show();
    _settingsWindowOpened=true;
  }
}

void Controller::directCopyHKTriggered()
{
  FakeKey::simulateCopy();
  _clipboard->clear(QClipboard::Clipboard);
}

//-------------------------------------------END OF Hotkey activation controls (Slots)---------------------------------


//-------------------------------------------basic functions------------------------------------------------------------

/**
 * @brief this is the function that calls first
 */
void Controller::start()
{
  GSettings::initialize();
  createViews();
  createHotkeys();
  createConnections();
  showViews();
}


void Controller::addClipboardContentToHistory()
{
  const QMimeData *MD=_clipboard->mimeData();
  foreach (QString str, MD->formats())
  {
      qDebug()<<str;
  }
//  DataFile *dataFile=new DataFile(MD,Resources::tempFolder.path()+"/SRL.txt");
//  QByteArray *BR=dataFile->data("text/plain");
//  qDebug()<<"PRINTING PLAIN TEXT_______________________________";
//  qDebug()<<QString(*BR);
//  delete BR;
//  BR=dataFile->at(1);
//  qDebug()<<"PRINTING HTML_______________________________";
//  qDebug()<<QString(*BR);
//  delete BR;
//  QFile file(Resources::tempFolder.path()+"/TEST.genius");
//  file.open(QFile::ReadWrite);
//  QDataStream stream(&file);
//  int i=0;
//  foreach (QString str, MD->formats())
//  {

//    QFile file_(Resources::tempFolder.path()+QString("/TEST_%1.genius").arg(i));
//    file_.open(QFile::ReadWrite);
//    qDebug()<<"Writing "<<str;
//    qDebug()<<"Wrote "<<file_.write(MD->data(str))<<" bytes";
//    file_.close();
//    i++;
//  }
//  qDebug()<<file.size();
//  file.close();
  ClipboardItem *item=NULL;
  if(MD->hasImage())
    item=new ClipboardImageItem(_clipboard->image());
  else if(MD->hasText())
    item=new ClipboardTextItem(_clipboard->text());
  else if(MD->hasHtml())
    item=new ClipboardTextItem(MD->html());
  else if(MD->hasUrls())
    item=new ClipboardURLItem(MD->urls());
  else return;
  if(item && item->constructed())
    _history->pushFront(item);
  else
    return;
}

/**
 * @brief define views
 */
void Controller::createViews()
{
  _manager=new Manager(_history,_clipboard);
  _manager->initialize();
  _trayIcon=new TrayIcon(_history);
  _selector=new Selector(_history);
  _settingsWindow=new SettingsWindow();
}

void Controller::showViews()
{
  if(!_managerOpened && !GSettings::openMinimized)
  {
    _manager->show();
  }
  _trayIcon->show();
}

/**
 * @brief add item to the view and history
 * @param itemto add
 * @param index of the new item
 */
void Controller::addItem(ClipboardItem *item, int index)
{

  if(!item)return;
  int reference=item->ref();
  ClipboardItem::ClipboardMimeType type=item->type();
  if(type==ClipboardItem::Text)
  {
   ClipboardTextItem *te=dynamic_cast<ClipboardTextItem*>(item);
   QString *text=new QString(*te->preview());
   if(GSettings::showInSingleLine)
     ToolKit::removeNewLines(text);
   if(GSettings::limitcharLength)
   {
     if(text->length()>GSettings::limitedCharLength)
       *text=text->left(GSettings::limitedCharLength)+"...";
   }
   QString tooltipText="type : text"+QString("\ncontent length : %1").arg(te->length())+QString("\nadded time : %1 ").arg(te->addedTime()->toString("hh.mm.ss.zzz AP"));
   _manager->addTextItem(text,&tooltipText,reference,index);
   _trayIcon->addTextAction(text,&tooltipText,reference,index);
   delete text;
  }
  else if(type==ClipboardItem::Image)
  {
    ClipboardImageItem *ii=dynamic_cast<ClipboardImageItem*>(item);

    QImage *image=ii->preview();
    QIcon icon(QPixmap::fromImage(*image));
    QString text(QString("width : %1").arg(ii->width()));
    text+=QString("  height : %1").arg(ii->hight());
    text+="  added time : "+ii->addedTime()->toString("hh.mm.ss.zzz AP");

    _manager->addImageItem(&text,&icon,reference,index);
    _trayIcon->addImageAction(&text,&icon,reference,index);
  }
  else if(type==ClipboardItem::URLs)
  {
    ClipboardURLItem *ui=dynamic_cast<ClipboardURLItem*>(item);
    QString *text=new QString(ui->toString("|"));
    if(GSettings::showInSingleLine)
      ToolKit::removeNewLines(text);
    if(GSettings::limitcharLength)
    {
      if(text->length()>GSettings::limitedCharLength)
        *text=text->left(GSettings::limitedCharLength)+"...";
    }

    QString tooltipText="type : urls"+QString("\nurls : %1").arg(ui->urls()->length())+QString("\nadded time : %1 ").arg(ui->addedTime()->toString("hh.mm.ss.zzz AP"));
    _manager->addTextItem(text,&tooltipText,reference,index);
    _trayIcon->addTextAction(text,&tooltipText,reference,index);
    delete text;
  }
}

/**
 * @brief create Global hotkeys acofing to the settings
 */
void Controller::createHotkeys()
{
  _openSelectorHotkey=new QHotkey(QKeySequence("Ctrl+Shift+V"),true);

  if(GSettings::clearHistoryHotKeyEnabled)
     _clearHistoryHotKey=new QHotkey(GSettings::clearHistoryHotKey,true);

  if(GSettings::pasteLastHotKeyEnabled)
     _pasteLastHotKey=new QHotkey(GSettings::pasteLastHotKey,true);

  if(GSettings::openManagerHotkeyEnabled)
     _openManagerHotKey=new QHotkey(GSettings::openManagerHotkey,true);

  if(GSettings::openSettingsHotKeyEnabled)
     _openSettingsHotKey=new QHotkey(GSettings::openSettingsHotKey,true);

  if(GSettings::directCopyHotKeyEnabled)
     _directCopyHotkey=new QHotkey(GSettings::directCopyHotKey,true);

  if(GSettings::historyMenuHotkeyEnabled)
    _historyMenuHotKey=new QHotkey(GSettings::historyMenuHotkey,true);

}

/**
 * @brief create connections betweeen application elements
 */
void Controller::createConnections()
{
  //-----------------------------------clipboardConnection
  connect(_clipboard,SIGNAL(changed(QClipboard::Mode)),this,SLOT(clipboardChanged(QClipboard::Mode)));

  //-----------------------------------connection with clipboardHistory
  connect(_history,SIGNAL(added(ClipboardItem*,int)),this,SLOT(history_itemAdded(ClipboardItem*,int)));
  connect(_history,SIGNAL(removed(int,int)),this,SLOT(history_removed(int,int)));
  connect(_history,SIGNAL(cleared()),this,SLOT(history_cleared()));
  connect(_history,SIGNAL(updated(ClipboardItem*)),this,SLOT(history_itemUpdated(ClipboardItem*)));

  //------------------------------------connection with clipboard Manager
  connect(_manager,SIGNAL(shown()),this,SLOT(manager_shown()));
  connect(_manager,SIGNAL(hidden()),this,SLOT(manager_hidden()));
  connect(_manager,SIGNAL(hidden()),_trayIcon,SLOT(managerHidden()));
  connect(_manager,SIGNAL(shown()),_trayIcon,SLOT(managerShown()));
  connect(_manager,SIGNAL(settingsDialogRequested()),this,SLOT(settingsWindowRequested()));
  connect(_manager,SIGNAL(itemSelected(int)),this,SLOT(itemSelected(int)));
  connect(_manager,SIGNAL(editRequested(ClipboardItem*)),this,SLOT(editRequested(ClipboardItem*)));

  //------------------------------------connection with TrayIcon
  connect(_trayIcon,SIGNAL(showHideManagerTriggerd()),this,SLOT(showHideManagerRequest()));
  connect(_trayIcon,SIGNAL(itemSelected(int)),this,SLOT(itemSelected(int)));
  connect(_trayIcon,SIGNAL(settingsDialogRequested()),this,SLOT(settingsWindowRequested()));
  connect(_trayIcon,SIGNAL(turnOffGenius()),this,SLOT(turnOffRequest()));
  connect(_trayIcon,SIGNAL(turnOnGenius()),this,SLOT(turnOnRequest()));
  connect(_trayIcon,SIGNAL(exitRequested()),this,SLOT(exitRequested()));

  //----------------------------------------connection with selector
  connect(_selector,SIGNAL(closing(int)),this,SLOT(selectorClosed(int)));

  //----------------------------------------connecction with settingsWindow
  connect(_settingsWindow,SIGNAL(hiding()),this,SLOT(settingsWindow_hidden()));

  //-----------------------------------------connection with globel hotkeys(all are conditional)
  if(_openSelectorHotkey)
    connect(_openSelectorHotkey,SIGNAL(activated()),this,SLOT(openSelectorHKtriggered()));

  if(_clearHistoryHotKey)
    connect(_clearHistoryHotKey,SIGNAL(activated()),this,SLOT(clearHistoryHKTrigered()));

  if(_pasteLastHotKey)
    connect(_pasteLastHotKey,SIGNAL(activated()),this,SLOT(pasteLasteHKTrigered()));

  if(_openManagerHotKey)
    connect(_openManagerHotKey,SIGNAL(activated()),this,SLOT(openManagerHKTriggered()));

  if(_openSettingsHotKey)
    connect(_openSettingsHotKey,SIGNAL(activated()),this,SLOT(openSettingsHKTriggered()));

  if(_directCopyHotkey)
    connect(_directCopyHotkey,SIGNAL(activated()),this,SLOT(directCopyHKTriggered()));

  if(_historyMenuHotKey)
    connect(_historyMenuHotKey,SIGNAL(activated()),this,SLOT(historyMenuHotkeyActivated()));
}

/**
 * @brief select an item from history and bring it to top
 * @param reference reference of selecte Item
 */
void Controller::selectItem(int reference)
{
  if(reference>-1)
  {
    if(!_history->isEmpty())
    {
      ClipboardItem *item=_history->get(reference);
      if(item)
      {
        ClipboardItem::ClipboardMimeType type=item->type();
        if(type==ClipboardItem::Text)
        {
          QString str=*dynamic_cast<ClipboardTextItem*>(item)->text();
          _history->remove(reference);
          _clipboard->setText(str);
        }
        else if(type==ClipboardItem::Image)
        {
          QImage img=*dynamic_cast<ClipboardImageItem*>(item)->image();
          _history->remove(reference);
          _clipboard->setImage(img);
        }
        else if(type==ClipboardItem::URLs)
        {
          QMimeData *mimedata=new QMimeData();
          mimedata->setUrls(*dynamic_cast<ClipboardURLItem*>(item)->urls());
          _history->remove(reference);
          _clipboard->setMimeData(mimedata);
        }
        else{}
      }
    }
  }
}

/**
 * @brief let the user to edit selected item
 * @param item for edit
 */
void Controller::letToEditItem(ClipboardItem *item)
{
  if(item)
  {
    ClipboardItem::ClipboardMimeType type=item->type();
    if(type==ClipboardItem::Text)
    {
      ClipboardTextItem *TI=dynamic_cast<ClipboardTextItem*>(item);
      QString temp(*TI->text());
      QString *text=new QString(temp);
      TextEditor textEditor(text);
      bool accept=textEditor.exec();
      if(accept)
      {
        if(text)
        {
          if(*text!=temp)
          {
            TI->text(*text);
            _history->itemUpdated(TI);
          }
        }
      }
      delete text;
    }
    else if(type==ClipboardItem::Image)
    {

      ImageEditor IE(dynamic_cast<ClipboardImageItem*>(item));
      bool accept=IE.exec();
      if(accept)
      {
         _history->itemUpdated(item);
      }
    }
  }
}

/**
 * @brief update given item , update views
 * @param item to update
 */
void Controller::updateItem(ClipboardItem *item)
{
  if(item)
  {
    ClipboardItem::ClipboardMimeType type=item->type();
    if(type==ClipboardItem::Text)
    {
      ClipboardTextItem *te=dynamic_cast<ClipboardTextItem*>(item);
      QString *text=new QString(*te->preview());
      if(GSettings::showInSingleLine)
        ToolKit::removeNewLines(text);
      if(GSettings::limitcharLength)
      {
        if(text->length()>GSettings::limitedCharLength)
          *text=text->left(GSettings::limitedCharLength)+"...";
      }
      QString tooltipText="type : text"+QString("\ncontent length : %1").arg(te->length())+QString("\nadded time : %1 ").arg(te->addedTime()->toString("hh.mm.ss.zzz AP"));
      int ref=te->ref();
      _manager->updateTextItem(text,&tooltipText,ref);
      _trayIcon->updateTextItem(text,&tooltipText,ref);
      delete text;
    }
    else if(type==ClipboardItem::Image)
    {
      ClipboardImageItem *ii=dynamic_cast<ClipboardImageItem*>(item);
      QImage *image=ii->preview();
      QIcon icon(QPixmap::fromImage(*image));
      QString text(QString("width : %1").arg(ii->width()));
      text+=QString("  height : %1").arg(ii->hight());
      text+="  added time : "+ii->addedTime()->toString("hh.mm.ss.zzz AP");
      int ref=ii->ref();
      _manager->updateImageItem(&text,&icon,ref);
      _trayIcon->updateImageItem(&text,&icon,ref);
    }
    else if(type==ClipboardItem::URLs)
    {
      ClipboardURLItem *ui=dynamic_cast<ClipboardURLItem*>(item);
      QString *text=new QString(ToolKit::URlsToString(ui->urls()));
      if(GSettings::showInSingleLine)
        ToolKit::removeNewLines(text);
      if(GSettings::limitcharLength)
      {
        if(text->length()>GSettings::limitedCharLength)
          *text=text->left(GSettings::limitedCharLength)+"...";
      }

      QString tooltipText="type : urls"+QString("\nurls : %1").arg(ui->urls()->length())+QString("\nadded time : %1 ").arg(ui->addedTime()->toString("hh.mm.ss.zzz AP"));
      int ref=ui->ref();
      _manager->updateTextItem(text,&tooltipText,ref);
      _trayIcon->updateTextItem(text,&tooltipText,ref);
      delete text;
    }
  }
}

/**
 * @brief check clipboard for empty
 * @return
 */
bool Controller::isClipboardEmpty()
{
  const QMimeData *mimeData=_clipboard->mimeData(QClipboard::Clipboard);
  if(!mimeData)
    return true;
  if(mimeData->hasText() || mimeData->hasImage() || mimeData->hasUrls())
    return false;
  else return true;
}


bool Controller::sameDataAgain()
{
  if(!_history||_history->isEmpty())return false;
  ClipboardItem *item=_history->first();
  ClipboardItem::ClipboardMimeType type=item->type();
  if(_clipboard->mimeData()->hasText() && type==ClipboardItem::Text)
      return  *dynamic_cast<ClipboardTextItem*>(item)->text()==_clipboard->text();
  else if(_clipboard->mimeData()->hasHtml() && type==ClipboardItem::Text)
      return *dynamic_cast<ClipboardTextItem*>(item)->text()==_clipboard->mimeData()->text();
  else if(_clipboard->mimeData()->hasImage() && type==ClipboardItem::Image)
      return *dynamic_cast<ClipboardImageItem*>(item)->image()==_clipboard->image();
  else if(_clipboard->mimeData()->hasUrls() && type==ClipboardItem::URLs)
      return *dynamic_cast<ClipboardURLItem*>(item)->urls()==_clipboard->mimeData()->urls();
  else return false;
}

/**
 * @brief unregister and delete all the Hotkeys
 */
void Controller::deleteHotkeys()
{

  if(_openSelectorHotkey)
  {
     _openSelectorHotkey->setRegistered(false);
     delete _openSelectorHotkey;
  }

  if(_clearHistoryHotKey)
  {
     _clearHistoryHotKey->setRegistered(false);
     delete _clearHistoryHotKey ;
  }

  if(_pasteLastHotKey)
  {
     _pasteLastHotKey->setRegistered(false);
     delete _pasteLastHotKey;
  }

  if(_openManagerHotKey)
  {
     _openManagerHotKey->setRegistered(false);
     delete _openManagerHotKey;
  }

  if(_openSettingsHotKey)
  {
     _openSettingsHotKey->setRegistered(false);
     delete _openSettingsHotKey;
  }

  if(_directCopyHotkey)
  {
     _directCopyHotkey->setRegistered(false);
     delete _directCopyHotkey;
  }

  if(_historyMenuHotKey)
  {
    _historyMenuHotKey->setRegistered(false);
    delete _historyMenuHotKey;
  }
}

/**
 * @brief delete views and variables
 */
void Controller::deleteVariables()
{
  if(_manager)
    delete _manager;

  if(_history)
    delete _history;

  if(_trayIcon)
    delete _trayIcon;

  if(_selector)
    delete _selector;
}
//----------------------------------------------view functions-------------------------------------------------------------

/**
 * @brief toggle the visible status of the Manager window.
 */
void Controller::toggleManager()
{
  if(_managerOpened)
    _manager->hide();
  else
    _manager->show();
}


void Controller::historyMenuHotkeyActivated()
{
  _trayIcon->showHistoryMenu();
}

void Controller::enableHotkeys(bool enable)
{
  if(_openSelectorHotkey)
     _openSelectorHotkey->setRegistered(enable);


  if(_clearHistoryHotKey)
     _clearHistoryHotKey->setRegistered(enable);


  if(_pasteLastHotKey)
     _pasteLastHotKey->setRegistered(enable);

  if(_openManagerHotKey)
     _openManagerHotKey->setRegistered(enable);


  if(_openSettingsHotKey)
     _openSettingsHotKey->setRegistered(enable);

  if(_directCopyHotkey)
     _directCopyHotkey->setRegistered(enable);


  if(_historyMenuHotKey)
    _historyMenuHotKey->setRegistered(enable);

}





