#ifndef TRAYICON_H
#define TRAYICON_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QSystemTrayIcon>
#include <QPoint>
#include <models/clipboardhistory.h>
#include <resources/resources.h>
#include <views/about.h>
#include <controllers/fakekey.h>

class TrayIcon : public QWidget
{
  Q_OBJECT
public:
  explicit TrayIcon(ClipboardHistory *history, QWidget *parent = 0);
  void show();
  void addTextAction(QString *text, QString *tooltipText, int reference, int index);
  void addImageAction(QString *text, QIcon *icon,int reference,int index);
  void updateTextItem(QString *text,QString *tooltipText,int reference);
  void updateImageItem(QString *text,QIcon *icon,int reference );
  void removeItem(int reference);
  void clearHistoryList();
  void showMessage(QString title,QString message,QSystemTrayIcon::MessageIcon icon,int duration);

  /**
   * @brief show the HistoryMenu then user can select an item . then it will call simulatePaste()
   */
  void showHistoryMenu();
  /**
   * @brief exchange locations of two actions in the historyMenu
   * @param entity 1 ref
   * @param entity 2 ref
   */
  void exchangeLocation(int ref1,int ref2);

signals:
  void showHideManagerTriggerd();
  void itemSelected(int reference);
  void settingsDialogRequested();
  void turnOffGenius();
  void turnOnGenius();
  void exitRequested();
public slots:
 void managerShown();
 void managerHidden();
 void menuActionTrigered(QAction *action);
 void historyMenuActionTriggered(QAction *action);

private:
  QSystemTrayIcon *_icon=NULL;
  QMenu *_menu=NULL;
  ClipboardHistory *_history=NULL;
  bool *_managerOpen;

  QMenu *_historyMenu=NULL;
  QAction *_showHideAction=NULL;
  QAction *_exitAction=NULL;
  QAction *_clearAction=NULL;
  QAction *_settingsAction=NULL;
  QAction *_onOffAction=NULL;
  QAction *_aboutAction=NULL;

  /**
   * @brief paste when a item in historyMenu is selected
   * if this is true whene item selected in historyMenu it will pasted to the active window
   */
  bool _pasteWhenSelected;
  void constructIcon();
  void checkLimit();


};

#endif // TRAYICON_H
