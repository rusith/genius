#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H
#include <QFileDialog>
#include <QDialog>
#include <resources/resources.h>
#include <QFile>
#include <QByteArray>
#include <QString>
#include <QMessageBox>
#include <QTextCodec>

namespace Ui {
class TextEditor;
}

class TextEditor : public QDialog
{
  Q_OBJECT

public:
  explicit TextEditor(QString *str,QWidget *parent = 0);
  ~TextEditor();


private slots:
  void on_cancelButton_clicked();
  void on_okButton_clicked();

  void on_importButton_clicked();

  void on_exportButton_clicked();

private:
  Ui::TextEditor *ui;
  QString *_str;
};

#endif // TEXTEDITOR_H
