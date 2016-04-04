#ifndef MANAGER_H
#define MANAGER_H

#include <QMainWindow>
#include <QDebug>
#include <QClipboard>
#include <QIcon>
#include <QCloseEvent>
#include <QListWidgetItem>
#include <QMenu>
#include <QFileSystemModel>
#include <QDir>
#include <models/clipboardhistory.h>
#include <tools/toolkit.h>
#include <resources/resources.h>

namespace Ui {
class Manager;
}
/**
 * @brief this user interface is used for manage items in the historry(edit/Delete)
 */
class Manager : public QMainWindow
{
  Q_OBJECT

public:
  /**
   * @brief initialize manager
   * @param history for interact with
   * @param parent of the object
   * @param open UI as hidden
   */
  explicit Manager(ClipboardHistory *history, QWidget *parent=0);
  ~Manager();

  /**
   * @brief add text item to the managed
   * @param text to show in item
   * @param tooltip text of the item
   * @param referenceID to save in items data (this is used when referenceing items)
   * @param index for add item
   */
  void addTextItem(QString *text,QString *tooltipText,int reference, int index);
  /**
   * @brief add item with a image
   * @param text to show with the image
   * @param icon to show
   * @param referenceID to save in items data (this is used when referenceing items)
   * @param index to add item
   */
  void addImageItem(QString *text,QIcon *icon,int reference,int index );
  /**
   * @brief remove item from manager using reference ID
   * @param refID of the item
   */
  void removeItem(int reference);
  /**
   * @brief clear the list of items
   */
  void clearList();

  /**
   * @brief initialize the window this will initialize  the UI
   */
  void initialize();

  /**
   * @brief echange locations of two entities
   * @param ref1
   * @param ref2
   */
  void exchangeLocations(int ref1,int ref2);
signals:
  /**
   * @brief emitted when the window is reopend
   */
  void shown();
  /**
   * @brief emitted when the window is hidden
   */
  void hidden();
  /**
   * @brief emitted when the UI requested the settings dialog(when clicking the settings button)
   */
  void settingsDialogRequested();
  /**
   * @brief emitted when user select an item(doubleclick)
   * @param refID of selected entity
   */
  void itemSelected(int reference);
  /**
   * @brief emitted when user want to view the content of an entity
   * @param entity
   */
  void showContentRequested(ClipboardEntity *entity);

  /**
   * @brief emited when user exchange location of two items
   * @param item 1 ref
   * @param item2 ref
   */
  void locationExchangeRequested(int ref1,int ref2);

private slots:
  /**
   * @brief calls when edit buton clicked
   */
  void on_deleteButton_clicked();
  /**
   * @brief calls when settings button clicked
   */
  void on_settingsButton_clicked();
  /**
   * @brief calls when clear button clicked
   */
  void on_clearButton_clicked();
  /**
   * @brief calls when user selected an item of the history list(double click)
   * @param selected item
   */
  void on_list_itemDoubleClicked(QListWidgetItem *item);

  /**
   * @brief calls when user request context menu of the list (rightclick)
   * @param mouse possition
   */
  void on_list_customContextMenuRequested(const QPoint &pos);
  /**
   * @brief calls when list-->contextMenu->actiontruggered (this connection is a temp connection )
   * @param action
   */
  void itemContextMenuTriggered(QAction *action);

  /**
   * @brief calls when user clicks the
   */
  void on_showContentButton_clicked();

  void on_list_currentRowChanged(int currentRow);

private:

  /**
   * @brief base UI object
   */
  Ui::Manager *ui;
  /**
   * @brief ClipboardHistory poibter for application's main history
   */
  ClipboardHistory *_history;
  /**
   * @brief start the window hidden
   */
  //bool _starthidden;
  /**
   * @brief initialize the UI with its commponents
   */
  void initializeUI();
  /**
   * @brief overridedd showEvent
   * @param event
   */
  void showEvent(QShowEvent *event);
  /**
   * @brief overridden hideEvent
   * @param event
   */
  void hideEvent(QHideEvent *event);
  /**
   * @brief overrridden closeEvent
   * @param event
   */
  void closeEvent(QCloseEvent *event);

  /**
   * @brief update item inforamtion label
   */
  void updateInfoLabel(const int &currentRow);



};

#endif // MANAGER_H
