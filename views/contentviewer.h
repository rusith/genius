#ifndef CONTENTVIEWER_H
#define CONTENTVIEWER_H

#include <QDialog>
#include <QListWidgetItem>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextBrowser>
#include <QTabWidget>
#include <QColorDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <models/clipboardentity.h>
#include <resources/resources.h>

namespace Ui {
class ContentViewer;
}
/**
 * @brief this dialog can show content of an ClipboardEntity .
 */
class ContentViewer : public QDialog
{
  Q_OBJECT

public:
  /**
   * @brief initialize ContentViewer user interface with provided ClipboardEntity object
   * @param ClipboardEntity for show contents
   * @param basic parent
   */
  explicit ContentViewer(ClipboardEntity *entity,QWidget *parent=0);
  ~ContentViewer();

private slots:
  void on_pushButton_clicked();

private:
  /**
   * @brief basic UI object
   */
  Ui::ContentViewer *ui;
  /**
   * @brief the ClipboardEntity currently showing contents
   */
  ClipboardEntity *_entity;

  /**
   * @brief initialize basic non dynamic eliments of the dialog using ClipboardEntity
   */
  void initializeBasicUI();

  /**
   * @brief add plainText tab to the tabView
   */
  void addPlainTextTab();

  /**
   * @brief add HTML tab to the window
   */
  void addHTMLTab();

  /**
   * @brief add image Tab to the window
   */
  void addImageTab();

  /**
   * @brief get user friendly text of an mimeType text
   * @param MimeType text
   * @return QString
   */
  QString imageMimeTypeToText(const QString &MT);
};

#endif // CONTENTVIEWER_H
