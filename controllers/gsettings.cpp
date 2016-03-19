#include "gsettings.h"

GSettings::GSettings()
{

}
void GSettings::initialize()
{
  QSettings settings("rusith","Genius");
  showInSingleLine=settings.value("showInSingleLine").toBool();
  limitcharLength=settings.value("limitViewCharactorLength").toBool();
  limitedCharLength=settings.value("charactorLimitOfItem").toInt();
  maximumItemsInHistory=settings.value("maximumItemsInHistory").toInt();
  saveImages=settings.value("saveImages").toBool();
  saveUrls=settings.value("saveURLs").toBool();
  openMinimized=settings.value("openMinimized").toBool();
  pasteAutomaticlay=settings.value("pasteAutomaticaly").toBool();
}

void GSettings::commit()
{
  QSettings settings("rusith","Genius");
  settings.setValue("showInSingleLine",showInSingleLine);
  settings.setValue("limitViewCharactorLength",limitcharLength);
  settings.setValue("charactorLimitOfItem",limitedCharLength);
  settings.setValue("maximumItemsInHistory",maximumItemsInHistory);
  settings.setValue("saveImages",saveImages);
  settings.setValue("saveURLs",saveUrls);
  settings.setValue("openMinimized",openMinimized);
  settings.setValue("pasteAutomaticaly",pasteAutomaticlay);
}

bool GSettings::showInSingleLine=false;
bool GSettings::limitcharLength=false;
int GSettings::limitedCharLength=0;
int GSettings::maximumItemsInHistory=0;
bool GSettings::saveImages=false;
bool GSettings::saveUrls=false;
bool GSettings::openMinimized=false;
bool GSettings::pasteAutomaticlay=false;
