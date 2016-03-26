#ifndef CLIPBOARDTEXTITEM_H
#define CLIPBOARDTEXTITEM_H
#include <QClipboard>
#include <QMimeData>
#include <QFile>
#include <QByteArray>
#include <models/clipboarditem.h>
#include <controllers/tempfolder.h>
#include <controllers/gsettings.h>

/**
 * @brief The ClipboardTextItem class is a type of ClipboardItem . that constins an text in it. this text is  maintained as a file if nessesery inside Genius temp folder. file will
 * automatically deleted when deleting this object . small texts are saved in dynamic memory.(not saved as file) [user can change the maximum size]
 * file name will be %RefIDTEXT.png
 */
class ClipboardTextItem :public ClipboardItem
{
public:
  /**
   * @brief construct object using clipboard reference
   * clipboard's current text will be the object's text
   * if clipboard dont have text on it text will be empty text
   * @param clipboard pointer
   */
  ClipboardTextItem(QClipboard *clipboard);
  /**
   * @brief construct object using a string
   * given string will be the text on of the object
   * @param text for the object
   */
  ClipboardTextItem(const QString &text);

  /**
   * @brief construct object using a string
   * given string will be the text on of the object
   * @param text for the object
   */
  ClipboardTextItem(QString *text);


  ~ClipboardTextItem();
  /**
   * @brief set object's text
   * @param new text
   */
  void text(const QString &text);

  /**
   * @brief return's object's text reference must delete after using
   * @return QString object's text
   */
  QString *text();

  /**
   * @brief the text sample
   * @return QString pointer
   */
  QString *preview();

  /**
   * @brief use to get text length of the item
   * @return text length
   */
  int length();


private:

  /**
   * @brief object's text
   */
  QString *_text=NULL;

  /**
   * @brief file that the text is saved
   */
  QFile *_textFile=NULL;

  /**
   * @brief length of the original text
   */
  int _length;
  /**
   * @brief delete the text file if it exists
   */
  void deleteTextFile();
};

#endif // CLIPBOARDTEXTITEM_H
