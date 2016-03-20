#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QApplication>
#include "settingsprovider.h"
#include <views/manager.h>
#include <models/clipboardhistory.h>
#include <QObject>
#include <QHotkey>
#include <QThread>
#include <tools/toolkit.h>
#include <views/trayicon.h>
#include <views/settings.h>
#include <views/texteditor.h>
#include <views/imageeditor.h>
#include <controllers/gsettings.h>
#include <views/selector.h>
#include <views/settingswindow.h>
#include <controllers/fakekey.h>

class Controller : public QObject
{
  Q_OBJECT
public:
  explicit Controller(QObject *parent = 0);
  ~Controller();
  void start();

signals:
private slots:
  //--------------------------basic control slots
  void clipboardChanged(QClipboard::Mode mode);
  void itemSelected(int reference);
  void editRequested(ClipboardItem *item);

  //---------------------------history slots
  void history_itemAdded(ClipboardItem *item,int index);
  void history_removed(int reference,int index);
  void history_cleared();
  void history_itemUpdated(ClipboardItem *item);

  //----------------------------views related slots
  void settingsdialogRequested();
  void showHideManagerRequest();
  void selectorClosed(int currentIndex);

  void manager_shown();
  void manager_hidden();

  void settingsWindow_hidden();

  //---------------------------------hotKey slots
  void openSelectorHKtriggered();
  void clearHistoryHKTrigered();
  void pasteLasteHKTrigered();
  void openManagerHKTriggered();
  void openSettingsHKTriggered();
  void directCopyHKTriggered();

private:
  //------------------------basic elements
  ClipboardHistory *_history;
  QClipboard *_clipboard;

  //------------------------window controls
  bool _managerOpened;
  bool _selectorOpen;
  bool _settingsWindowOpened;
  bool _holtCollection;

  //-------------------------view elements
  Manager *_manager;
  TrayIcon *_trayIcon;
  Selector *_selector;
  SettingsWindow *_settingsWindow;


  //----------------------------hotkeys
  QHotkey *_openSelectorHotkey;
  QHotkey *_clearHistoryHotKey;
  QHotkey *_pasteLastHotKey;
  QHotkey *_openManagerHotKey;
  QHotkey *_openSettingsHotKey;
  QHotkey *_directCopyHotkey;

  //------------------------------functions
  void createHotkeysAndConnections();
  void makeConnections();
  bool sameDataAgain();
  bool isClipboardEmpty();

  void selectItemWithoutDeleting(int reference);
};

#endif // CONTROLLER_H
