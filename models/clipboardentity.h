#ifndef CLIPBOARDENTITY_H
#define CLIPBOARDENTITY_H

#include <QClipboard>
#include <QMimeData>
#include <QTime>
#include <models/filing/datafile.h>
#include <resources/resources.h>
#include <controllers/gsettings.h>

/**
 * @brief The ClipboardEntity class represent an clipboar history entyty. it holds a mimeData object as a file
 * file delees with object
 */
class ClipboardEntity
{
public:
  /**
   * @brief this constructor constructs the object using QClipboard reference (using its current QMimeData)
   * @param QClipboard reference use for get data
   */
  ClipboardEntity(QClipboard *clipboard);
  /**
   * @brief this constructor generetes the object using QMimeData
   * @param mimedata to use
   */
  ClipboardEntity(const QMimeData *mimeData);

  ~ClipboardEntity();

  /**
   * @brief get automatic reference ID of the object
   * @return reference id
   */
  int ref();
  /**
   * @brief auto incrementing reference ID . this ID is automaticaly incrementing by one when creating new Enity
   */
  static int refID;

  /**
   * @brief check the two object's data is same (this calls identicalData function on _dataFile reference )
   * @param entity for check same
   * @return true or false
   */
  bool identical(ClipboardEntity *entity);

  /**
   * @brief get data as QMimeData object (must be deleted after using)
   * @return new QMimeData (must delete after using) NULL if no data
   */
  QMimeData *data();
  /**
   * @brief check the object has HTML inmemory
   * @return
   */
  bool hasHTML();
  /**
   * @brief check the object have image
   * @return true or false
   */
  bool hasImage();
  /**
   * @brief check the obejc has plainText content
   * @return true or false
   */
  bool hasPlainText();
  /**
   * @brief get plain text of the object
   * @return pointer to object's plaintext object
   */
  const QString *plainText();
  /**
   * @brief get object's HTML text
   * @return pointer to object's HTML text
   */
  const QString *HTMLText();
  /**
   * @brief get pointer's scaled image
   * @return pointer to the object's Image
   */
  const QImage *image();
  /**
   * @brief get created time of the object
   * @return QTime ptr
   */
  const QTime *addedTime();

  /**
   * @brief get plaintext of the object . if no plaintext present in the object an empty string will return
   * @param check plainstring present
   * @param length of needed text (-1 if full content)
   * @return QSTring plaintext
   */
  QString plainText(bool check,int length);
private:
  /**
   * @brief refId of the object
   */
  int _ref;
  /**
   * @brief data of the object
   */
  DataFile *_dataFile=NULL;
  /**
   * @brief object's plainText
   */
  QString *_plainText=NULL;
  /**
   * @brief object's HTML Text
   */
  QString *_HTMLText=NULL;
  /**
   * @brief object's Image
   */
  QImage *_image=NULL;

  /**
   * @brief created time
   */
  QTime *_addedTime=NULL;
  /**
   * @brief set object's content(_plainText,_image etc) using given DataFile
   * @param DataFile to use
   */
  void setContent(DataFile *file);
};

#endif // CLIPBOARDENTITY_H
