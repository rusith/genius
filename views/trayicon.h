#ifndef TRAYICON_H
#define TRAYICON_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <models/clipboardhistory.h>
#include <QAction>
#include <QDebug>
#include <resources/resources.h>
#include <views/settings.h>
#include <QSystemTrayIcon>

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
signals:
  void showHideManagerTriggerd();
  void itemSelected(int reference);
public slots:
 void managerShown();
 void managerHidden();
 void menuActionTrigered(QAction *action);
 void historyMenuActionTriggered(QAction *action);

private:
  QSystemTrayIcon *_icon;
  QMenu *_menu;
  ClipboardHistory *_history;
  bool *_managerOpen;

  QMenu *_historyMenu;
  QAction *_showHideAction;
  QAction *_exitAction;
  QAction *_clearAction;

  void constructIcon();
};

#endif // TRAYICON_H
