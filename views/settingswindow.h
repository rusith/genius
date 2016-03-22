#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QMainWindow>
#include <QShowEvent>
#include <QHideEvent>
#include <QCloseEvent>
#include <QColorDialog>
#include <QColor>
#include <QDebug>
#include <controllers/gsettings.h>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit SettingsWindow(QWidget *parent = 0);
  ~SettingsWindow();

signals:
  void hiding();

private slots:
  void on_saveButton_clicked();
  void on_cancelButton_clicked();
  void on_SBCSelectButton_clicked();
  void on_BorderColorButton_clicked();
  void on_animationDurationInput_textEdited(const QString &arg1);

private:
  Ui::SettingsWindow *ui;

  //------------------------------basic functions
  void initializeUI();
  void initializeElements();
  void saveData();

  //----------------------------------events
  void showEvent(QShowEvent *event);
  void hideEvent(QHideEvent *event);
  void closeEvent(QCloseEvent *event);
};

#endif // SETTINGSWINDOW_H
