#include "trayicon.h"

TrayIcon::TrayIcon(ClipboardHistory *history, QWidget *parent) : QWidget(parent)
{
  if(history)
    _history=history;
  constructIcon();
}

void TrayIcon::constructIcon()
{
   _icon=new QSystemTrayIcon(this);
   _menu=new QMenu(this);

  _icon->setIcon(QIcon(Resources::logo16));

  _historyMenu=new QMenu(_menu);
  _historyMenu->setTitle("history");
  _historyMenu->setIcon(QIcon(Resources::history16));
  _menu->addMenu(_historyMenu);

  _showHideAction=new QAction(_menu);
  _showHideAction->setIcon(QIcon(Resources::logo16));
  _showHideAction->setIconText("show clipboard manager");
  _menu->addAction(_showHideAction);

  _clearAction=new QAction(_menu);
  _clearAction->setIcon(QIcon(Resources::clrea16));
  _clearAction->setIconText("clear hisory");
  _menu->addAction(_clearAction);

  _settingsAction=new QAction(_menu);
  _settingsAction->setIcon(QIcon(Resources::settings16));
  _settingsAction->setIconText("settings");
  _settingsAction->setToolTip("open settings dialog");
  _menu->addAction(_settingsAction);

  _onOffAction=new QAction(_menu);
  _onOffAction->setIconText("turn off");
  _onOffAction->setIcon(QIcon(Resources::on16));
  _onOffAction->setToolTip("turn off genius . new items will not added to the list until turn on");
  _menu->addAction(_onOffAction);

  _exitAction=new QAction(_menu);
  _exitAction->setIcon(QIcon(Resources::exit16));
  _exitAction->setIconText("exit");
  _exitAction->setToolTip("exit from Genius");
  _menu->addAction(_exitAction);

  connect(_historyMenu,SIGNAL(triggered(QAction*)),this,SLOT(historyMenuActionTriggered(QAction*)));
  connect(_menu,SIGNAL(triggered(QAction*)),this,SLOT(menuActionTrigered(QAction*)));
  _icon->setContextMenu(_menu);
  _icon->setToolTip("system tray icon of genius ");
}

//slots
void TrayIcon::managerHidden()
{
   _showHideAction->setIconText("show clipboard manager");
}

void TrayIcon::managerShown()
{
  _showHideAction->setIconText("hide clipboard manager");
}

void TrayIcon::show()
{
  _icon->show();
}


//functions
void TrayIcon::addImageAction(QString *text, QIcon *icon, int reference, int index)
{

  QAction *action=new QAction(_historyMenu);
  action->setIcon(*icon);
  action->setIconText(*text);
  action->setData(QVariant(reference));
  if(_historyMenu->actions().isEmpty())
    _historyMenu->addAction(action);
  else
  {
    if(index==0)
    {
       _historyMenu->insertAction(_historyMenu->actions().first(),action);
    }
    else
    {
      if(_historyMenu->actions().length()>=index)
      {
        _historyMenu->insertAction(_historyMenu->actions().at(index),action);
      }
      else
      {
       _historyMenu->insertAction(_historyMenu->actions().first(),action);
      }
    }
  }

  checkLimit();
}

void TrayIcon::addTextAction(QString *text, QString *tooltipText, int reference, int index)
{

  QAction *action=new QAction(_historyMenu);
  action->setText(*text);
  action->setToolTip(*tooltipText);
  action->setData(QVariant(reference));
  if(_historyMenu->actions().isEmpty())
    _historyMenu->addAction(action);
  else
  {
    if(index==0)
    {
       _historyMenu->insertAction(_historyMenu->actions().first(),action);
    }
    else
    {
      if(_historyMenu->actions().length()>=index)
      {
        _historyMenu->insertAction(_historyMenu->actions().at(index),action);
      }
      else
      {
       _historyMenu->insertAction(_historyMenu->actions().first(),action);
      }
    }
  }

  checkLimit();
}

void TrayIcon::updateTextItem(QString *text, QString *tooltipText, int reference)
{

  if(_historyMenu->actions().length()>0)
  {
    int ref=0;
    foreach (QAction *action, _historyMenu->actions())
    {
      if(action)
      {
        ref=action->data().toInt();
        if(ref==reference)
        {
          action->setText(*text);
          action->setToolTip(*tooltipText);
          return;
        }
      }
    }
  }
}

void TrayIcon::updateImageItem(QString *text, QIcon *icon, int reference)
{
  if(_historyMenu->actions().length()>0)
  {
    int ref=0;
    foreach (QAction *action, _historyMenu->actions())
    {
      if(action)
      {
        ref=action->data().toInt();
        if(ref==reference)
        {
          action->setIcon(*icon);
          action->setIconText(*text);
          return;
        }
      }
    }
  }
}

void TrayIcon::removeItem(int reference)
{
  int length=_historyMenu->actions().length();
  if(length>0)
  {
    QAction *action;
    for(int i=0;i<length;i++)
    {
      action=_historyMenu->actions().at(i);
      if(action)
      {
        int ref=action->data().toInt();
        if(ref==reference)
        {
          _historyMenu->actions().removeAt(i);
          delete action;
          return;
        }
      }
    }
  }

}

void TrayIcon::clearHistoryList()
{
  if(_historyMenu)
  {
    int length=_historyMenu->actions().length();
    if(length>0)
    {

      foreach (QAction *action, _historyMenu->actions())
      {
        delete action;
      }
      _historyMenu->actions().clear();
    }
  }
}

void TrayIcon::menuActionTrigered(QAction *action)
{
  if(action==_exitAction)
  {
    exit(0);
  }
  else if(action==_showHideAction)
  {
    emit showHideManagerTriggerd();
  }
  else if(action==_clearAction)
  {
    if(_history && _history->isEmpty()==false)
      _history->clear();
  }
  else if(action==_settingsAction)
  {
    emit settingsDialogRequested();
  }
  else if(action==_onOffAction)
  {
    if(_onOffAction->iconText()=="turn off")
    {
      _onOffAction->setIconText("turn on");
      _onOffAction->setIcon(QIcon(Resources::off16));
      _onOffAction->setToolTip("turn on genius . new items will saved in the history");
      emit turnOffGenius();

    }
    else
    {
      _onOffAction->setIconText("turn off");
      _onOffAction->setIcon(QIcon(Resources::on16));
      _onOffAction->setToolTip("turn off genius . new items will not added to the list until turn on");
      emit turnOnGenius();
    }
  }
}

void TrayIcon::historyMenuActionTriggered(QAction *action)
{
  if(action)
  {
    int reference=action->data().toInt();
    emit itemSelected(reference);
  }
}

void TrayIcon::showMessage(QString title, QString message, QSystemTrayIcon::MessageIcon icon, int duration)
{
  if(_icon)
  {
    _icon->showMessage(title,message,icon,duration);
  }
}


void TrayIcon::checkLimit()
{
  if(GSettings::historyItemLimit<_historyMenu->actions().count())
  {
    while (_historyMenu->actions().count()>GSettings::historyItemLimit)
    {
      delete _historyMenu->actions().takeLast();
    }
  }
}
