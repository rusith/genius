#ifndef CLIPBOARDITEM_H
#define CLIPBOARDITEM_H

#include <QClipboard>
#include <QImage>
#include <QList>
#include <QDebug>
#include <QUrl>
#include <QMimeData>
#include <QTime>
#include <QTemporaryFile>
#include <QPixmap>
#include <resources/resources.h>

class ClipboardItem
{
public:
  /**
   * @brief main constructor (reference ID is creating here)
   */
  ClipboardItem();

  /**
    * @brief main destructor
    */
  virtual ~ClipboardItem();
  /**
   * @brief holds next itemReference ID
   */
  static int clipboardItemRef;

  /**
   * @brief represents 3 CLipboardItemTypes
   */
  enum ClipboardMimeType{Image,Text,URLs,GnomeCopiedFiles};

  /**
   * @brief get the type of the object
   * @return ClipboardMimeType (Image/Text/URLs)
   */
  ClipboardMimeType type();

  /**
   * @brief get the reference ID of the clipboardItemObject
   * @return int referenceID (_ref)
   */
  int ref();

  /**
   * @brief successfully constructed or not. if false the object contains error. cannot use it.
   * @return bool constructed
   */
  bool constructed();

  /**
   * @brief time of this object created
   * @return Qtime creatad time
   */
  QTime *addedTime();
protected:

  /**
   * @brief used to get type of the object . assingned in constructor of the sub classes
   */
  ClipboardMimeType _type;

  /**
   * @brief item's reference id
   */
  int _ref;

  /**
   * @brief time the item was created
   */
  QTime *_addedTime;

  /**
   * @brief the object is successfully constructed or not
   */
  bool _constructed;
//  ClipboardItem();
//  ClipboardItem(QClipboard *clipboard);
//  ~ClipboardItem();

//  //getters
//  QImage image();
//  QImage *imagePreview();
//  int imageWidth();
//  int imageHight();
//  bool constructedSuccessfully();
//  QString *text();
//  QList<QUrl> *urls();
//  const QTime *addedTime();
//  ClipboardMimeType type();
//  const int &ref();

//  //setters
//  void image(QImage &image);
//  void text(QString &tex);
//  void urls(QList<QUrl> &url);

//  //functions
//  void setContent(QClipboard *clipboard);

//private:
//  ClipboardMimeType _type;

//  //private fealds
//  QFile *_imageFile=NULL;
//  QImage *_image=NULL;
//  QString *_text=NULL;
//  QString *_html=NULL;
//  QList<QUrl> *_urls=NULL;
//  QTime _addedTime;
//  int _ref;
//  bool _inFile;
//  int _imageWidth=0;
//  int _imageHight=0;
//  bool _constructedSuccessfully;


//  //private functions
//  void setTimeToNow();
};

#endif // CLIPBOARDITEM_H
