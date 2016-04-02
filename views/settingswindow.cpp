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
  ui->ShowInSingleLineCheckBox->setChecked(GSettings::showInSingleLine);
  ui->maximumItemsInHistory->setValue(GSettings::maximumItemsInHistory);
  ui->HistoryMenuLimit->setValue(GSettings::historyItemLimit);
  if(GSettings::limitcharLength)
  {
      ui->LimitTextLengthCheckbox->setChecked(true);
      ui->TextLimitSpinBox->setValue(GSettings::limitedCharLength);
  }
  else
      ui->LimitTextLengthCheckbox->setChecked(false);


  ui->pasteAutomaticalyButoon->setChecked(GSettings::pasteAutomaticlay);

  ui->clearHistory->setKeySequence(GSettings::clearHistoryHotKey);
  ui->pasteLast->setKeySequence(GSettings::pasteLastHotKey);
  ui->openManager->setKeySequence(GSettings::openManagerHotkey);
  ui->openSettings->setKeySequence(GSettings::openSettingsHotKey);

  ui->clearHistoryCB->setChecked(GSettings::clearHistoryHotKeyEnabled);
  ui->pasteLastCB->setChecked(GSettings::pasteLastHotKeyEnabled);
  ui->openManagerCB->setChecked(GSettings::openManagerHotkeyEnabled);
  ui->openSettingsCB->setChecked(GSettings::openSettingsHotKeyEnabled);

  ui->SBCPreview->setStyleSheet("QLabel{\n	background:"+GSettings::selectorItemBackgroundColor+";\n background-repeat: repeat-y;\n  background-position: left;\n}");
  ui->SBCPreview->setText(GSettings::selectorItemBackgroundColor);
  ui->textColorPreview->setStyleSheet("QLabel{\n	background:"+GSettings::selectorTextColor+";\n background-repeat: repeat-y;\n  background-position: left;\n}");
  ui->textColorPreview->setText(GSettings::selectorTextColor);

  ui->borderSizeSpinBox->setValue(GSettings::selectorBorderSize);
  ui->BorederColorPreview->setStyleSheet("QLabel{\n	background:"+GSettings::selectorBorderColor+";\n background-repeat: repeat-y;\n  background-position: left;\n}");
  ui->BorederColorPreview->setText(GSettings::selectorBorderColor);
  ui->animationDurationInput->setText(QString("%1").arg(GSettings::selectorAnimationDuration));
  ui->BorderRadiusSpinBox->setValue(GSettings::selectorBorderRadius);

  ui->sbImageWidth->setValue(GSettings::maximumImageWidth);
  ui->sbImageHieght->setValue(GSettings::maximumImageHight);

  ui->historyMenuHotkey_kse->setKeySequence(GSettings::historyMenuHotkey);
  ui->historyMenu_cb->setChecked(GSettings::historyMenuHotkeyEnabled);
}

void SettingsWindow::saveData()
{
  GSettings::openMinimized=ui->openMinimizedCheckbox->isChecked();
  GSettings::saveImages=ui->saveImages->isChecked();
  GSettings::showInSingleLine=ui->ShowInSingleLineCheckBox->isChecked();
  GSettings::maximumItemsInHistory=ui->maximumItemsInHistory->value();
  GSettings::historyItemLimit=ui->HistoryMenuLimit->value();
  if(ui->LimitTextLengthCheckbox->isChecked())
  {
      GSettings::limitcharLength=true;
      GSettings::limitedCharLength=ui->TextLimitSpinBox->value();
  }
  else
    GSettings::limitcharLength=false;


  GSettings::pasteAutomaticlay=ui->pasteAutomaticalyButoon->isChecked();

  GSettings::clearHistoryHotKey=ui->clearHistory->keySequence();
  GSettings::pasteLastHotKey=ui->pasteLast->keySequence();
  GSettings::openManagerHotkey=ui->openManager->keySequence();
  GSettings::openSettingsHotKey=ui->openSettings->keySequence();

  GSettings::clearHistoryHotKeyEnabled=ui->clearHistoryCB->isChecked();
  GSettings::pasteLastHotKeyEnabled=ui->pasteLastCB->isChecked();
  GSettings::openManagerHotkeyEnabled=ui->openManagerCB->isChecked();
  GSettings::openSettingsHotKeyEnabled=ui->openSettingsCB->isChecked();

  GSettings::selectorItemBackgroundColor=ui->SBCPreview->text();
  GSettings::selectorTextColor=ui->textColorPreview->text();
  GSettings::selectorBorderColor=ui->BorederColorPreview->text();
  GSettings::selectorBorderSize=ui->borderSizeSpinBox->value();
  GSettings::selectorAnimationDuration=ui->animationDurationInput->text().toInt();
  GSettings::selectorBorderRadius=ui->BorderRadiusSpinBox->value();

  GSettings::maximumImageWidth=ui->sbImageWidth->value();
  GSettings::maximumImageHight=ui->sbImageHieght->value();

  GSettings::historyMenuHotkeyEnabled=ui->historyMenu_cb->isChecked();
  GSettings::historyMenuHotkey=ui->historyMenuHotkey_kse->keySequence();

  GSettings::commit();
}

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
  event->ignore();
  hide();
}

void SettingsWindow::on_saveButton_clicked()
{
  saveData();
  hide();
}

void SettingsWindow::on_cancelButton_clicked()
{
  hide();
}

void SettingsWindow::on_SBCSelectButton_clicked()
{
    QColorDialog dialog;
    dialog.setWindowTitle("select a color for selector items background");
    if(dialog.exec())
    {
      QColor color=dialog.selectedColor();
      QString name=color.name();
      ui->SBCPreview->setStyleSheet("QLabel{\n	background:"+name+";\n background-repeat: repeat-y;\n  background-position: left;\n}");
      ui->SBCPreview->setText(name);
    }

}

void SettingsWindow::on_BorderColorButton_clicked()
{
  QColorDialog dialog;
  dialog.setWindowTitle("select a color for selector items border");
  if(dialog.exec())
  {
    QColor color=dialog.selectedColor();
    QString name=color.name();
    ui->BorederColorPreview->setStyleSheet("QLabel{\n	background:"+name+";\n background-repeat: repeat-y;\n  background-position: left;\n}");
    ui->BorederColorPreview->setText(name);
  }
}

void SettingsWindow::on_animationDurationInput_textEdited(const QString &arg1)
{

  int res=arg1.toInt();
  if(res==0)
  {
    ui->animationDurationInput->setText("1");
    ui->animationDurationInput->selectAll();
  }
  else if(res>10000 || res<1)
  {
    ui->animationDurationInput->setText("1");
    ui->animationDurationInput->selectAll();
  }
}

void SettingsWindow::on_textColorChangeButton_clicked()
{
  QColorDialog dialog;
  dialog.setWindowTitle("select a color for selector items text");
  if(dialog.exec())
  {
    QColor color=dialog.selectedColor();
    QString name=color.name();
    ui->textColorPreview->setStyleSheet("QLabel{\n	background:"+name+";\n background-repeat: repeat-y;\n  background-position: left;\n}");
    ui->textColorPreview->setText(name);
  }
}

void SettingsWindow::on_defaultButton_clicked()
{
  setToDefault();
}

void SettingsWindow::setToDefault()
{
  GSettings::resetToDefault();
  initializeElements();
}
