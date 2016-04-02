#ifndef CONTROLLER_H
#define CONTROLLER_H


#include <QObject>
#include <QDebug>
#include <QApplication>
#include <QObject>
#include <QHotkey>
#include <QThread>
#include <QBuffer>
#include <views/manager.h>
#include <models/clipboardhistory.h>
#include <tools/toolkit.h>
#include <views/trayicon.h>
#include <views/settingswindow.h>
#include <views/selector.h>
#include <views/contentviewer.h>
#include <controllers/gsettings.h>
#include <controllers/tempfolder.h>
#include <models/filing/datafile.h>
#include <controllers/fakekey.h>

class Controller : public QObject
{
  Q_OBJECT
public:
  explicit Controller(QObject *parent = 0);
  ~Controller();
  void start();

  static const int WAIT_BEFORE_PASTE=100;

signals:
private slots:
  //--------------------------basic control slots
  /**
   * @brief calls when the clipboard is changed effect only when CLipboar is chaned && not holted && not samedata again
   * @param mode of the change (effect inly if mode is Clipboard)
   */
  void clipboardChanged(QClipboard::Mode mode);
  void itemSelected(int reference);


  void history_itemAdded(ClipboardEntity *entity,int index);
  void history_removed(int reference,int index);
  void history_cleared();

  void settingsWindowRequested();
  void showHideManagerRequest();
  void selectorClosed(int currentIndex);

  void manager_shown();
  void manager_hidden();

  void settingsWindow_hidden();
  void turnOffRequest();
  void turnOnRequest();
  void exitRequested();

  void openSelectorHKtriggered();
  void clearHistoryHKTrigered();
  void pasteLasteHKTrigered();
  void openManagerHKTriggered();
  void openSettingsHKTriggered();

  /**
   * @brief calls when _historyMenuHotKey activated. (connected using signal and slots)
   */
  void historyMenuHotkeyActivated();

  /**
   * @brief open content viewer and show content to the user
   * @param entity for show
   */
  void showContent(ClipboardEntity *entity);

  void locationExchangeRequested(int ref1,int ref2);
  void locationExchanged(int ref1,int ref2);


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

  /**
   * @brief hotkey for show history menu (inside the trayIcon). then user can select item from the menu
   */
  QHotkey *_historyMenuHotKey=NULL;

  //------------------------------ basic functions

  /**
   * @brief add current content in the System clipboard to History.
   */
  void addClipboardContentToHistory();
  void createViews();
  void showViews();
  void addItem(ClipboardEntity *entity, int index);
  void createHotkeys();
  void createConnections();
  /**
   * @brief calls when user selected an item selected item's data wii set to the device's clipboard
   * @param reference of the item
   */
  void selectItem(int reference);
  //void letToEditItem(ClipboardItem *item);
  //void updateItem(ClipboardItem *item);
  //--------------------------------view function
  void toggleManager();
  void makeConnections();

  /**
   * @brief check the system's clipboard is empty
   * @return true or false
   */
  bool isClipboardEmpty();
  void deleteHotkeys();
  void deleteVariables();

  /**
   * @brief enable or disable all hotkeys
   * @param enable or disable
   */
  void enableHotkeys(bool enable);

  /**
   * @brief check the first item of the history is same as given item
   * @param entity for check
   * @return true or false
   */
  bool sameAsLast(ClipboardEntity *entity);




};

#endif // CONTROLLER_H
