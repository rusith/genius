#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QCloseEvent>
#include <controllers/gsettings.h>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
  Q_OBJECT

public:
  explicit Settings(QWidget *parent = 0);
  ~Settings();

private slots:
  void on_saveButton_clicked();

private:
  Ui::Settings *ui;

  void initializeUI();
};

#endif // SETTINGS_H
