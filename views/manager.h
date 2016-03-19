#ifndef MANAGER_H
#define MANAGER_H

#include <QMainWindow>
#include <QDebug>
#include <QClipboard>
#include <QIcon>
#include <QCloseEvent>
#include <QListWidgetItem>
#include <QMenu>
#include <controllers/settingsprovider.h>
#include <models/clipboardhistory.h>
#include <tools/toolkit.h>
#include <resources/resources.h>

namespace Ui {
class Manager;
}

class Manager : public QMainWindow
{
  Q_OBJECT

public:
  explicit Manager( ClipboardHistory *history, QClipboard *clipboard, QWidget *parent=0,bool hidden=false);
  //Manager(QWidget *parent = 0);
  ~Manager();

  void addTextItem(QString *text,QString *tooltipText,int reference, int index);
  void addImageItem(QString *text,QIcon *icon,int reference,int index );
  void removeItem(int reference);
  void clearList();
  void updateTextItem(QString *text,QString *tooltipText,int reference);
  void updateImageItem(QString *text,QIcon *icon,int reference );
  void initialize();
public slots:
signals:
  void shown();
  void hidden();
  void settingsDialogRequested();
  void itemSelected(int reference);
  void editRequested(ClipboardItem *item);
private slots:
  void on_deleteButton_clicked();

  void on_settingsButton_clicked();

  void on_clearButton_clicked();

  void on_list_itemDoubleClicked(QListWidgetItem *item);

  void on_editButton_clicked();

  void on_list_customContextMenuRequested(const QPoint &pos);

  void itemContextMenuTriggered(QAction *action);
private:
  // fields
  Ui::Manager *ui;
  ClipboardHistory *_history;
  QClipboard *_clipboard;
  bool _starthidden;
  //functions
  void initializeUI();
  void addItem(ClipboardItem *item, int index);
  void setupSettings(SettingsProvider *settings);


  void showEvent(QShowEvent *event);
  void hideEvent(QHideEvent *event);
  void closeEvent(QCloseEvent *event);

};

#endif // MANAGER_H
