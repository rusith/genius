#ifndef CLIPBOARDIMAGEITEM_H
#define CLIPBOARDIMAGEITEM_H
#include <QClipboard>
#include <QFile>
#include <QImage>
#include <models/clipboarditem.h>
#include <controllers/gsettings.h>
#include <controllers/tempfolder.h>

/**
 * @brief The ClipboardImageItem class is a type of ClipboardItem . that constins an image in it. this image maintained as a file inside Genius temp folder file will
 * automatically deleted when deleting this object . small images are saved in dynamic memory.(not saved as file) [user can change the maximum size]
 * file name will be %RefIDIMG.png
 */
class ClipboardImageItem:public ClipboardItem
{
public:

  /**
   * @brief constructor that set image using clipboard object
   * @param clipboard object for get the image data
   */
  ClipboardImageItem(QClipboard *clipboard);

  /**
   * @brief constructor  get image from a QImage pointer
   * @param image
   */
  ClipboardImageItem(QImage *image);

  /**
   * @brief constructor that get the image from a QImage object
   * @param image
   */
  ClipboardImageItem(const QImage &image);

  /**
    *removes _image , _imageFiles
    */
  ~ClipboardImageItem();

  /**
   * @brief set the image (saving it in a tem file creating a sample)
   * @param image of the object
   * @return setting succesfull or not
   */
  bool image(QImage *image);

  /**
   * @brief get main image as a pointer for new image
   *        must be delete the result after using
   * @return pointer for a new image
   */
  QImage *image();

  /**
   * @brief returns the preview (scaled version of the original Image) . this can use for show in UI
   * @return QIMage Pointer
   */
  QImage *preview();

  /**
   * @brief use to get the width of the image
   * @return int width of the image
   */
  int width();

  /**
   * @brief use to get the hight of the image
   * @return int hight of the image
   */
  int hight();


private:

  /**
   * @brief image (preview)(in memory image)
   */
  QImage *_image=NULL;

  /**
   * @brief image file of the object
   */
  QFile *_imageFile=NULL;

  /**
   * @brief pixel width of the main image
   */
  int _width;
  /**
   * @brief pixel hight of the main image
   */
  int _hight;

  /**
   * @brief check the given image can store in the memory (<=10000 px)
   * @param image to check
   * @return can strore in the memory
   */
  bool CanStoreOnMemory(QImage *image);

  /**
   * @brief deletedeleteImageFile the existing imageFile and its object
   */
  void deleteImageFile();
};

#endif // CLIPBOARDIMAGEITEM_H
