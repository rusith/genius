#ifndef DATAFILE_H
#define DATAFILE_H

#include <QDebug>
#include <QHash>
#include <QByteArray>
#include <QMimeData>
#include <QFile>
#include <QTime>
#include <QScopedPointer>
#include <QImage>
#include <QUrl>
#include <QStringList>
#include <QElapsedTimer>
#include <tools/rtimer.h>
#include <tools/toolkit.h>
#include <models/filing/fragmentframe.h>

/**
 * @brief The DataFile class provides interface for manage QMimeData object as a file
 * this file is saved as given file name
 */
class DataFile
{
public:
  /**
   * @brief only constructor
   * @param mime data to save as a file
   * @param file name to save
   */
  DataFile(const QMimeData *mimeData,const QString &filename);

  ~DataFile();
  /**
   * @brief get data relevant to given format the returned reference must delete after using it
   * @param format for get data
   * @return new QByteArray object --> its reference (this must be delete after using)
   * return NULL if not available
   * return empty ByteArray if data is empty
   */
  QByteArray *data(const QString &format) const;

  /**
   * @brief use to get format data of given object
   * @param index for get data
   * @return new QByteArray object --> its reference (this must be delete after using)
   * return NULL if not a valid index
   * return empty ByteArray f data is empty
   */
  QByteArray *at(const int &index);

  /**
   * @brief use to check the no data
   * @return true if no data otherwise false
   */
  bool isEmpty();

  /**
   * @brief how much formats in this object
   * @return format count
   */
  int count();

  /**
   * @brief use to get the file name of this object
   * @return the file name of the object
   */
  QString fileName();

  /**
   * @brief convert file into QMimeData object (object must delete after using)
   * @return QMimeData reference NULL if something wrong
   */
  QMimeData *toMimeData();

  /**
   * @brief check the object has plain text type on it
   * @return true--> has plain text
   *         false--> no plain text
   */
  bool hasPlainText();

  /**
   * @brief check the object has HTML text
   * @return true--> has HTML text
   *         false--> no HTML text
   */
  bool hasHtmlText();

  /**
   * @brief check the object has image on it
   * @return true--> has Image
   *         false--> no image
   */
  bool hasImage();


  /**
   * @brief get plain text of the object . if no plain text present in the object an empty string will return
   * @param check plain string present
   * @param length of needed text (-1 if full content)
   * @return QSTring plain text
   */
  QString plainText(bool check, int length);

  /**
   * @brief check HTML text of the object if no HTML text present an empty string will return
   * @param check format is available (not need check is already know because this not changing)
   * @param length of needed text (-1 if full content)
   * @return QString HTML Text if available . otherwise empty QString
   */
  QString HTMLText(bool check, int length);

  /**
   * @brief get image data from the file
   * @param check an image is available
   * @param specify 0> for get scaled image
   * @param specify 0> for get scaled image
   * @return QImage object (must delete after using)
   */
  QImage *image(bool check,const int &width, const int &hight);

  /**
   * @brief this function checks the given dataFile object's _fragment and this object's fragment's are same
   * if so return true else false <b>this function not consider about actual data . only fragments</b>
   * @param file to check are the same
   * @return true if same otherwise false
   */
  bool identicalData(DataFile *file);

  /**
   * @brief returns formats supported by this content
   * @return list of mime formats
   */
  QStringList formats();

  /**
   * @brief used to get byte count of specific format
   * @param format for get size
   * @return size of givent format if format not availabel -->0;
   */
  quint64 formatSize(const QString &format);

  /**
   * @brief used to get only present image formats
   * @return
   */
  QStringList imageFormats();

  /**
   * @brief overrided operator for check equelity
   * @param right hand side object
   * @return true if equal
   */
  bool operator ==(const DataFile &rhs) const ;

  /**
   * @brief overrided operator for check equelity
   * @param right hand side object
   * @return true if equal
   */
  bool operator ==(DataFile *rhs) const;

  /**
   * @brief used to get size of the object's data
   * @return size
   */
  quint64 size();

  /**
   * @brief used to check the data file contais list of URLS
   * @return true or false
   */
  bool hasUrls();

  /**
   * @brief used to get list of urls from the data file
   * @return returns empty list if no urls availabel
   */
  QList<QUrl> urls();

private:
  /**
   * @brief _fragments (Hash for manage formats)
   */
  QHash<QString,FragmentFrame> *_fragments=NULL;

  /**
   * @brief file relevant to this object
   */
  QFile *_file=NULL;

  /**
   * @brief check one or more formats available in the object
   * @param format
   * @return true if available otherwise false
   */
  bool hasFormat(const QStringList &strlst);

  /**
   * @brief read given FragmentFrame into given char sequence from the file
   * @param frame to read
   * @param sequence to store
   * @return bytes read
   */
  qint64 readFragment(const FragmentFrame &frame, char *cha);

  /**
   * @brief read given fragment and return byte array (byteArray must delete manually)
   * @param frame to read
   * @return QByteArray
   */
  QByteArray *readFragment(const FragmentFrame &frame) const;
};

#endif // DATAFILE_H
