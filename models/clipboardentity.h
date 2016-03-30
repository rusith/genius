#ifndef CLIPBOARDENTITY_H
#define CLIPBOARDENTITY_H

#include <QClipboard>
#include <QMimeData>
#include <QTime>
#include <models/filing/datafile.h>
#include <resources/resources.h>
#include <controllers/gsettings.h>

/**
 * @brief The ClipboardEntity class represent an clipboard history entity. it holds a mimeData object as a file
 * file deletes with object
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
   * @brief this constructor generates the object using QMimeData
   * @param mime data to use
   */
  ClipboardEntity(const QMimeData *mimeData);

  ~ClipboardEntity();

  /**
   * @brief get automatic reference ID of the object
   * @return reference id
   */
  int ref();
  /**
   * @brief auto incrementing reference ID . this ID is automatically incrementing by one when creating new Entity
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
   * @brief used to get data for given format (returned object must delete manually)
   * @param format needed
   * @return new ByteArray
   */
  QByteArray *data(const QString &format);
  /**
   * @brief check the object has HTML in memory
   * @return
   */
  bool hasHTML();
  /**
   * @brief check the object have image
   * @return true or false
   */
  bool hasImage();
  /**
   * @brief check the object has plainText content
   * @return true or false
   */
  bool hasPlainText();
  /**
   * @brief get plain text of the object
   * @return pointer to object's plain text object
   */
  const QString *plainText();
  /**
   * @brief get object's HTML text
   * @return pointer to object's HTML text
   */
  const QString HTMLText(bool check, const int &length);

  /**
   * @brief get image from the object
   * @param check an image is available
   * @param specify 0> for get scaled image
   * @param specify 0> for get scaled image
   * @return QImage object reference (delete after using)
   */
  QImage *image(bool check, const int &width, const int &hight);
  /**
   * @brief get created time of the object
   * @return QTime ptr
   */
  const QTime *addedTime();

  /**
   * @brief get plain text of the object . if no plain text present in the object an empty string will return
   * @param check plain string present
   * @param length of needed text (-1 if full content)
   * @return QSTring plain text
   */
  QString plainText(bool check,int length);

  /**
   * @brief used to get formats supported by this objetc as QStringList
   * @return list of mime formats if something wrong . returns empty list
   */
  QStringList contentFormats();

  /**
   * @brief used to get byte count of specific format from the data
   * @param format for get size
   * @return size of givent format if format not availabel -->0;
   */
  quint64 formatSize(const QString &format);


  /**
   * @brief used to get only present image formats
   * @return
   */
  QStringList imageFormats();
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
   * @brief created time
   */
  QTime *_addedTime=NULL;
};

#endif // CLIPBOARDENTITY_H
