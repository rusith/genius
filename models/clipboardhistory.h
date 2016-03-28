#ifndef CLIPBOARDHISTORY_H
#define CLIPBOARDHISTORY_H

#include <QObject>
#include <QList>
#include <models/clipboarditem.h>
#include <models/clipboardentity.h>
#include <controllers/gsettings.h>

/**
 * @brief The ClipboardHistory class holds list of ClipboardEntitys
 */
class ClipboardHistory : public QObject
{
  Q_OBJECT
public:
  /**
   * @brief only constructor.(normal QObject constructor)
   * @param parent of the object
   */
  explicit ClipboardHistory(QObject *parent = 0);
  ~ClipboardHistory();
  /**
   * @brief append item to the lest of the list if list reached to max item count item will not added to the list
   * @param entity to add
   */
  void append(ClipboardEntity *entity);
  /**
   * @brief push item as the first item (to the top )
   * if(maximum item count reached last item will be deleted)
   * @param entity to add
   */
  void pushFront(ClipboardEntity *entity);
  /**
   * @brief remove enity using given reference
   * @param reference to delete
   */
  void remove(const int &reference);
  /**
   * @brief remove item at specific index (starting from 0)
   * @param index to remove
   */
  void removeAt(int index);
  /**
   * @brief clear the list add delete all the objects
   */
  void clear();
  /**
   * @brief used to check the list for empty
   * @return true is empty
   */
  bool isEmpty();
  /**
   * @brief item count in the object
   * @return
   */
  int length();

  /**
   * @brief get specific item using reference if this object is updateing manualy must call updated
   * @param refererence to get the object
   * @return pointer for specific object if(invalid reference id returns NULL)
   */
  ClipboardEntity *get(const int &reference);
  /**
   * @brief get specific item using index if this object is updateing manualy must call updated
   * @param index to get the object
   * @return pointer for specific object if(invalid index returns NULL)
   */
  ClipboardEntity *at(const int &index);
  /**
   * @brief returs first item of the list
   * @return ClipboardEntity first item if(empty) returns NULL
   */
  ClipboardEntity *first();

  /**
   * @brief this function shuld call if a entity (belongs to this object )
   * is updated autside. this function emits updated signal
   * @param updated
   */
  void itemUpdated(ClipboardEntity *entity);

  /**
   * @brief get index of a reference
   * @param reference
   * @return index of the reference if availabel otherwise -1
   */
  int indexOf(const int &reference);


signals:
  /**
   * @brief emited when an item is added
   * @param added entity
   * @param index of the entity
   */
  void added(ClipboardEntity *entity,int index);
  /**
   * @brief emited when an entity is removed
   * @param reference id of the entity
   * @param index of the entity
   */
  void removed(int reference,int index);
  /**
   * @brief emited when entity is updated
   * @param updated entity
   */
  void updated(ClipboardEntity *entity);
  /**
   * @brief wmited wen list is cleared
   */
  void cleared();

private:
  /**
   * @brief main list that hold all entities
   */
  QList<ClipboardEntity*> *_list;
  /**
   * @brief remove last items if limit reached
   */
  void manageLength();
};

#endif // CLIPBOARDHISTORY_H
