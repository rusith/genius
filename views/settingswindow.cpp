#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::SettingsWindow)
{
  ui->setupUi(this);
}

SettingsWindow::~SettingsWindow()
{
  delete ui;
}

//--------------------------------------basic functions
void SettingsWindow::initializeUI()
{
  ui->centralwidget->setLayout(ui->verticalLayout_2);
  setCentralWidget(ui->centralwidget);
}

void SettingsWindow::initializeElements()
{
  ui->openMinimizedCheckbox->setChecked(GSettings::openMinimized);
  ui->saveImages->setChecked(GSettings::saveImages);
  ui->saveURLs->setChecked(GSettings::saveUrls);
  ui->ShowInSingleLineCheckBox->setChecked(GSettings::showInSingleLine);
  ui->maximumItemsInHistory->setValue(GSettings::maximumItemsInHistory);
  if(GSettings::limitcharLength)
  {
      ui->LimitTextLengthCheckbox->setChecked(true);
      ui->TextLimitSpinBox->setValue(GSettings::limitedCharLength);
  }
  else
      ui->LimitTextLengthCheckbox->setChecked(false);


  ui->pasteAutomaticalyButoon->setChecked(GSettings::pasteAutomaticlay);

  ui->openSelector->setKeySequence(GSettings::openSelectorHotKey);
  ui->clearHistory->setKeySequence(GSettings::clearHistoryHotKey);
  ui->pasteLast->setKeySequence(GSettings::pasteLastHotKey);
  ui->openManager->setKeySequence(GSettings::openManagerHotkey);
  ui->openSettings->setKeySequence(GSettings::openSettingsHotKey);
  ui->directCopy->setKeySequence(GSettings::directCopyHotKey);

  ui->OpenSelectorCB->setChecked(GSettings::openSelectorHotKeyEnabled);
  ui->clearHistoryCB->setChecked(GSettings::clearHistoryHotKeyEnabled);
  ui->pasteLastCB->setChecked(GSettings::pasteLastHotKeyEnabled);
  ui->openManagerCB->setChecked(GSettings::openManagerHotkeyEnabled);
  ui->openSettingsCB->setChecked(GSettings::openSettingsHotKeyEnabled);
  ui->directCopyCB->setChecked(GSettings::directCopyHotKeyEnabled);

}

void SettingsWindow::saveData()
{
  GSettings::openMinimized=ui->openMinimizedCheckbox->isChecked();
  GSettings::saveImages=ui->saveImages->isChecked();
  GSettings::saveUrls=ui->saveURLs->isChecked();
  GSettings::showInSingleLine=ui->ShowInSingleLineCheckBox->isChecked();
  GSettings::maximumItemsInHistory=ui->maximumItemsInHistory->value();
  if(ui->LimitTextLengthCheckbox->isChecked())
  {
      GSettings::limitcharLength=true;
      GSettings::limitedCharLength=ui->TextLimitSpinBox->value();
  }
  else
    GSettings::limitcharLength=false;


  GSettings::pasteAutomaticlay=ui->pasteAutomaticalyButoon->isChecked();

  GSettings::openSelectorHotKey=ui->openSelector->keySequence();
  GSettings::clearHistoryHotKey=ui->clearHistory->keySequence();
  GSettings::pasteLastHotKey=ui->pasteLast->keySequence();
  GSettings::openManagerHotkey=ui->openManager->keySequence();
  GSettings::openSettingsHotKey=ui->openSettings->keySequence();
  GSettings::directCopyHotKey=ui->directCopy->keySequence();

  GSettings::openSelectorHotKeyEnabled=ui->OpenSelectorCB->isChecked();
  GSettings::clearHistoryHotKeyEnabled=ui->clearHistoryCB->isChecked();
  GSettings::pasteLastHotKeyEnabled=ui->pasteLastCB->isChecked();
  GSettings::openManagerHotkeyEnabled=ui->openManagerCB->isChecked();
  GSettings::openSettingsHotKeyEnabled=ui->openSettingsCB->isChecked();
  GSettings::directCopyHotKeyEnabled=ui->directCopyCB->isChecked();
}

//----------------------------------------------------------events
void SettingsWindow::showEvent(QShowEvent *event)
{
  initializeUI();
  initializeElements();
  event->accept();
}

void SettingsWindow::hideEvent(QHideEvent *event)
{
  emit hiding();
  event->accept();
}

void SettingsWindow::closeEvent(QCloseEvent *event)
{
  //ignore the close event and hide the window
  event->ignore();
  hide();
}

//--------------------------------------------slots
void SettingsWindow::on_saveButton_clicked()
{
  saveData();
  hide();
}

void SettingsWindow::on_cancelButton_clicked()
{
  hide();
}

