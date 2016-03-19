#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Settings)
{
  ui->setupUi(this);
  initializeUI();
}

Settings::~Settings()
{
  delete ui;
}


void Settings::initializeUI()
{
  setLayout(ui->VL1);
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
  {
      ui->LimitTextLengthCheckbox->setChecked(false);
  }

  ui->pasteAutomaticalyButoon->setChecked(GSettings::pasteAutomaticlay);
}

void Settings::on_saveButton_clicked()
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
  {
    GSettings::limitcharLength=false;
  }

  GSettings::pasteAutomaticlay=ui->pasteAutomaticalyButoon->isChecked();

  accept();
}
