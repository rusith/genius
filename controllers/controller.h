#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QApplication>
#include <views/manager.h>
#include <models/clipboardhistory.h>
#include <QObject>
#include <QHotkey>
#include <QThread>
#include <tools/toolkit.h>
#include <views/trayicon.h>
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
  void settingsWindowRequested();
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
  ClipboardHistory *_history=NULL;
  QClipboard *_clipboard=NULL;

  //------------------------window controls
  bool _managerOpened;
  bool _selectorOpen;
  bool _settingsWindowOpened;
  bool _holtCollection;

  //-------------------------view elements
  Manager *_manager=NULL;
  TrayIcon *_trayIcon=NULL;
  Selector *_selector=NULL;
  SettingsWindow *_settingsWindow=NULL;


  //----------------------------hotkeys
  QHotkey *_openSelectorHotkey=NULL;
  QHotkey *_clearHistoryHotKey=NULL;
  QHotkey *_pasteLastHotKey=NULL;
  QHotkey *_openManagerHotKey=NULL;
  QHotkey *_openSettingsHotKey=NULL;
  QHotkey *_directCopyHotkey=NULL;

  //------------------------------ basic functions
  void createViews();
  void showViews();
  void addItem(ClipboardItem *item, int index);
  void createHotkeys();
  void createConnections();
  void selectItem(int reference);
  void letToEditItem(ClipboardItem *item);
  void updateItem(ClipboardItem *item);
  //--------------------------------view function
  void toggleManager();
  void makeConnections();
  bool sameDataAgain();
  bool isClipboardEmpty();
  void deleteHotkeys();
  void deleteVariables();
};

#endif // CONTROLLER_H
