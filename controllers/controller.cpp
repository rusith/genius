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
  if(mode==QClipboard::Clipboard )
  {
    if(!isClipboardEmpty() && !sameDataAgain() && !_holtCollection)
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
  if(_settingsWindowOpened)
    _settingsWindowOpened=false;
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

/**
 * @brief check last item and new item is same
 * @return
 */
bool Controller::sameDataAgain()
{
  if(!_history||_history->isEmpty())return false;
  ClipboardItem *item=_history->first();
  ClipboardItem::ClipboardMimeType type=item->type();
  if(_clipboard->mimeData()->hasText() && type==ClipboardItem::Text)
  {
      return *item->text()==_clipboard->text();
  }
  else if(_clipboard->mimeData()->hasImage() && type==ClipboardItem::Image)
  {
      return *item->image()==_clipboard->image();
  }
  else if(_clipboard->mimeData()->hasUrls() && type==ClipboardItem::URLs)
  {
      return *item->urls()==_clipboard->mimeData()->urls();
  }
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
