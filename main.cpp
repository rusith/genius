#include <QApplication>
#include <controllers/controller.h>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  Controller controller;
  controller.start();
  return a.exec();
}
