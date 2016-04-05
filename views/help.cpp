#include "help.h"
#include "ui_help.h"

Help::Help(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Help)
{
    ui->setupUi(this);
    setLayout(ui->rootLayout);
}

Help::~Help()
{
    delete ui;
}
