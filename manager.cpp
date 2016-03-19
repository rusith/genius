#include "manager.h"
#include "ui_manager.h"

Manager::Manager(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::Manager)
{
  ui->setupUi(this);
}

Manager::~Manager()
{
  delete ui;
}
