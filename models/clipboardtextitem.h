#ifndef CLIPBOARDTEXTITEM_H
#define CLIPBOARDTEXTITEM_H
#include <QClipboard>
#include <QMimeData>
#include <QFile>
#include <QByteArray>
#include <models/clipboarditem.h>
#include <controllers/controller.h>
#include <controllers/gsettings.h>
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
  ClipboardTextItem(QString &text);

  /**
   * @brief construct object using a string
   * given string will be the text on of the object
   * @param text for the object
   */
  ClipboardTextItem(QString \*text);

  /**
   * @brief set object's text
   * @param new text
   */
  void text(QString &text);

  /**
   * @brief return's object's text reference must delete after using
   * @return QString object's text
   */
  QString *text();


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
   * @brief delete the text file if it exists
   */
  void deleteTextFile();
};

#endif // CLIPBOARDTEXTITEM_H
