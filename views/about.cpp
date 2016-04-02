#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::About)
{
  ui->setupUi(this);
  this->setFixedSize(this->width(),this->height());
}

About::~About()
{
  delete ui;
}

void About::on_licenseButton_clicked()
{
  License li(0);
  li.exec();
}

void About::closeEvent(QCloseEvent *event)
{
  event->ignore();
  reject();
}

void About::on_pushButton_clicked()
{
   accept();
}
