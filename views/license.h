#ifndef LICENSE_H
#define LICENSE_H

#include <QDialog>
#include <QCloseEvent>
namespace Ui {
class License;
}

class License : public QDialog
{
  Q_OBJECT

public:
  explicit License(QWidget *parent = 0);
  ~License();

private:
  Ui::License *ui;
  void closeEvent(QCloseEvent *event);
};

#endif // LICENSE_H
