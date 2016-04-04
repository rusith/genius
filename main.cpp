#include <QApplication>
#include <QFile>
#include <QSharedMemory>
#include <QMessageBox>
#include <controllers/runguard.h>
#include <controllers/controller.h>

int main(int argc, char *argv[])
{

  QApplication a(argc, argv);

  RunGuard guard( "2c7d2328-cdb5-416d-9a5d-2d8a2ab4b87b" );
  if ( !guard.tryToRun() )
  {
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle("Genius is already running");
    msgBox.setText( QObject::tr("Cannot run multiple instances.\nAnother instance of Genius is already running.") );
    msgBox.exec();
    return 0;
  }

  a.setQuitOnLastWindowClosed(false);
  Controller controller;
  controller.start();

  return a.exec();

}
