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

/**
 * @brief use to manage files deferent fragments. this struct contains the size of the dragment,end,start byts
 */
class FragmentFrame
{
public:
  /**
   * @brief starting byte of the file fragment
   */
  quint64 start=0;
  /**
   * @brief size of the file fragment
   */
  quint64 size=0;

  bool operator==(const FragmentFrame &other) const
  {
    return (other.size==size && other.start==start);
  }
};

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
   * @brief get data relavent to given format the returned reference must delete after using it
   * @param format for get data
   * @return new QByteArray object --> its reference (this must be delete after using)
   * return NULL if not availabel
   * return empty ByteArray if data is empty
   */
  QByteArray *data(const QString &format);

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
   * @brief check the object has html text
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
   * @brief get plaintext of the object . if no plaintext present in the object an empty string will return
   * @param check plainstring present
   * @param length of needed text (-1 if full content)
   * @return QSTring plaintext
   */
  QString plainText(bool check, int length);

  /**
   * @brief check HTML text of the object if no HTML text present an empty string will return
   * @param check format is available (not need check is already know becouse this not changing)
   * @return QString HTML Text if avilabel . therwise empty QString
   */
  QString HTMLText(bool check);

  /**
   * @brief get image of the object
   * @param check
   * @return
   */
  QImage *image(bool check);

  /**
   * @brief this function checks the given dataFile object's _fragment and this object's fragmen's are same
   * if so return true else false <b>this function not consider aboud actual data . only fragments</b>
   * @param file to check are the same
   * @return true if same otherwise false
   */
  bool identicalData(DataFile *file);
private:
  /**
   * @brief _fragments (Hash for manage formats)
   */
  QHash<QString,FragmentFrame> *_fragments;

  /**
   * @brief file relevent to this object
   */
  QFile *_file;

  /**
   * @brief check one or more formats availabel in the object
   * @param format
   * @return true if availabe otherwise false
   */
  bool hasFormat(const QStringList &strlst);

  /**
   * @brief read given FragmentFrame into given char sequence from the file
   * @param frame to read
   * @param sequence to store
   * @return bytes readed
   */
  qint64 readFragment(const FragmentFrame &frame, char *cha);

  /**
   * @brief read given fragment and return bytearray (byteArray must delete manualy)
   * @param frame to read
   * @return QByteArray
   */
  QByteArray *readFragment(const FragmentFrame &frame);
};

#endif // DATAFILE_H
