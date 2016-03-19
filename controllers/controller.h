#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QApplication>
#include "settingsprovider.h"
#include <views/manager.h>
#include <models/clipboardhistory.h>
#include <QObject>
#include <tools/toolkit.h>
#include <views/trayicon.h>
#include <views/settings.h>
#include <views/texteditor.h>
#include <views/imageeditor.h>
#include <controllers/gsettings.h>
#include <views/selector.h>
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
  void clipboardChanged(QClipboard::Mode mode);
  void history_itemAdded(ClipboardItem *item,int index);
  void history_removed(int reference,int index);
  void history_cleared();
  void history_itemUpdated(ClipboardItem *item);
  //void settings_updated(SettingsProvider *settings);
  void settingsdialogRequested();
  void showHideManagerRequest();
  void itemSelected(int reference);
  void editRequested(ClipboardItem *item);
  void manager_shown();
  void manager_hidden();
  void openSelectorHKtriggered();
  void selectorClosed(int currentIndex);
private:
  //SettingsProvider *_settings;
  ClipboardHistory *_history;
  QClipboard *_clipboard;
  bool _managerOpened;
  bool _selectorOpen;

  Manager *_manager;
  TrayIcon *_trayIcon;
  Selector *_selector;
  QHotkey *_openSelectorHotkey;


  void makeConnections();
//  void setupSettings(SettingsProvider *settings);
  bool sameDataAgain();
};

#endif // CONTROLLER_H
