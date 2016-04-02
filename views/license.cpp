#include "license.h"
#include "ui_license.h"

License::License(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::License)
{
  ui->setupUi(this);
  setLayout(ui->verticalLayout);
}

License::~License()
{
  delete ui;
}

void License::closeEvent(QCloseEvent *event)
{
  event->ignore();
  reject();
}
