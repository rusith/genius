#include "gsettings.h"

GSettings::GSettings()
{

}
void GSettings::initialize()
{
  QSettings settings("rusith","Genius");
  QFile fSettings(settings.fileName());
  if(fSettings.exists()==false || fSettings.size()<1)
    resetToDefault();
  showInSingleLine=settings.value("showInSingleLine").toBool();
  limitcharLength=settings.value("limitViewCharactorLength").toBool();
  limitedCharLength=settings.value("charactorLimitOfItem").toInt();
  maximumItemsInHistory=settings.value("maximumItemsInHistory").toInt();
  saveImages=settings.value("saveImages").toBool();
  openMinimized=settings.value("openMinimized").toBool();
  pasteAutomaticlay=settings.value("pasteAutomaticaly").toBool();
  historyItemLimit=settings.value("historyItemLimit").toInt();

  clearHistoryHotKey=QKeySequence(settings.value("clearHistoryHotkey").toString());
  pasteLastHotKey=QKeySequence(settings.value("pasteLastHotkey").toString());
  openManagerHotkey=QKeySequence(settings.value("openManagerHotkey").toString());
  openSettingsHotKey=QKeySequence(settings.value("openSettingsHotkey").toString());


  clearHistoryHotKeyEnabled=settings.value("clearHistoryHotKeyEnabled").toBool();
  pasteLastHotKeyEnabled=settings.value("pasteLastHotKeyEnabled").toBool();
  openManagerHotkeyEnabled=settings.value("openManagerHotkeyEnabled").toBool();
  openSettingsHotKeyEnabled=settings.value("openSettingsHotKeyEnabled").toBool();
  selectorEnabled=settings.value("selectorEnabled").toBool();

  selectorItemBackgroundColor=settings.value("selectorItemBackgroundColor").toString();
  selectorTextColor=settings.value("selectorTextColor").toString();
  selectorBorderColor=settings.value("selectorBorderColor").toString();
  selectorBorderSize=settings.value("selectorBorderSize").toInt();
  selectorAnimationDuration=settings.value("selectorAnimationDuration").toInt();
  selectorBorderRadius=settings.value("selectorBorderRadius").toInt();

  maximumImageWidth=settings.value("maximumImageWidth").toInt();
  maximumImageHight=settings.value("maximumImageHight").toInt();


  historyMenuHotkeyEnabled=settings.value("historyMenuHotkeyEnabled").toBool();
  historyMenuHotkey=QKeySequence(settings.value("historyMenuHotkey").toString());

  showIndexesInHistoryMenu=settings.value("showIndexesInHistoryMenu").toBool();
  enableDeepSearch=settings.value("enableDeepSearch").toBool();
}

void GSettings::commit()
{
  QSettings settings("rusith","Genius");

  settings.setValue("showInSingleLine",showInSingleLine);
  settings.setValue("limitViewCharactorLength",limitcharLength);
  settings.setValue("charactorLimitOfItem",limitedCharLength);
  settings.setValue("maximumItemsInHistory",maximumItemsInHistory);
  settings.setValue("saveImages",saveImages);
  settings.setValue("openMinimized",openMinimized);
  settings.setValue("pasteAutomaticaly",pasteAutomaticlay);
  settings.setValue("historyItemLimit",historyItemLimit);

  settings.setValue("clearHistoryHotkey",clearHistoryHotKey.toString(QKeySequence::NativeText));
  settings.setValue("pasteLastHotkey",pasteLastHotKey.toString(QKeySequence::NativeText));
  settings.setValue("openManagerHotkey",openManagerHotkey.toString(QKeySequence::NativeText));
  settings.setValue("openSettingsHotkey",openSettingsHotKey.toString(QKeySequence::NativeText));


  settings.setValue("clearHistoryHotKeyEnabled",clearHistoryHotKeyEnabled);
  settings.setValue("pasteLastHotKeyEnabled",pasteLastHotKeyEnabled);
  settings.setValue("openManagerHotkeyEnabled",openManagerHotkeyEnabled);
  settings.setValue("openSettingsHotKeyEnabled",openSettingsHotKeyEnabled);
  settings.setValue("selectorEnabled",selectorEnabled);
  settings.setValue("selectorItemBackgroundColor",selectorItemBackgroundColor);
  settings.setValue("selectorTextColor",selectorTextColor);
  settings.setValue("selectorBorderColor",selectorBorderColor);
  settings.setValue("selectorBorderSize",selectorBorderSize);
  settings.setValue("selectorAnimationDuration",selectorAnimationDuration);
  settings.setValue("selectorBorderRadius",selectorBorderRadius);

  settings.setValue("maximumImageWidth",maximumImageWidth);
  settings.setValue("maximumImageHight",maximumImageHight);


  settings.setValue("historyMenuHotkeyEnabled",historyMenuHotkeyEnabled);
  settings.setValue("historyMenuHotkey",historyMenuHotkey.toString(QKeySequence::NativeText));

  settings.setValue("showIndexesInHistoryMenu",showIndexesInHistoryMenu);
  settings.setValue("enableDeepSearch",enableDeepSearch);
}

void GSettings::resetToDefault()
{
  showInSingleLine=true;
  limitcharLength=true;
  limitedCharLength=50;
  maximumItemsInHistory=40;
  saveImages=true;
  openMinimized=true;
  pasteAutomaticlay=true;
  historyItemLimit=20;

  clearHistoryHotKey=QKeySequence("Ctrl+Shift+Alt+C");
  pasteLastHotKey=QKeySequence("Ctrl+Alt+V");
  openManagerHotkey=QKeySequence("Ctrl+Shift+Alt+M");
  openSettingsHotKey=QKeySequence("Ctrl+Shift+Alt+S");
  historyMenuHotkey=QKeySequence("Ctrl+Shift+V");

  clearHistoryHotKeyEnabled=true;
  pasteLastHotKeyEnabled=true;
  openManagerHotkeyEnabled=true;
  openSettingsHotKeyEnabled=true;
  historyMenuHotkeyEnabled=true;
  selectorEnabled=true;


  selectorItemBackgroundColor="#5555ff";
  selectorTextColor="black";
  selectorBorderColor="blue";
  selectorBorderSize=2;
  selectorAnimationDuration=100;
  selectorBorderRadius=12;

  maximumImageWidth=50;
  maximumImageHight=50;

  showIndexesInHistoryMenu=true;
  enableDeepSearch=true;

  commit();
}


bool GSettings::showInSingleLine=false;
bool GSettings::limitcharLength=false;
int GSettings::limitedCharLength=0;
int GSettings::maximumItemsInHistory=0;
bool GSettings::saveImages=false;
bool GSettings::openMinimized=false;
bool GSettings::pasteAutomaticlay=false;
int GSettings::historyItemLimit=10;
QKeySequence GSettings::clearHistoryHotKey=QKeySequence();
QKeySequence GSettings::pasteLastHotKey=QKeySequence();
QKeySequence GSettings::openManagerHotkey=QKeySequence();
QKeySequence GSettings::openSettingsHotKey=QKeySequence();
QKeySequence GSettings::historyMenuHotkey=QKeySequence();
bool GSettings::clearHistoryHotKeyEnabled=false;
bool GSettings::pasteLastHotKeyEnabled=false;
bool GSettings::openManagerHotkeyEnabled=false;
bool GSettings::openSettingsHotKeyEnabled=false;
bool GSettings::historyMenuHotkeyEnabled=false;
QString GSettings::selectorItemBackgroundColor="";
QString GSettings::selectorTextColor="";
QString GSettings::selectorBorderColor="";
int GSettings::selectorBorderSize=1;
int GSettings::selectorAnimationDuration=100;
int GSettings::selectorBorderRadius=1;
int GSettings::maximumImageWidth=1;
int GSettings::maximumImageHight=1;
bool GSettings::selectorEnabled=true;
bool GSettings::showIndexesInHistoryMenu=true;
bool GSettings::enableDeepSearch=false;
