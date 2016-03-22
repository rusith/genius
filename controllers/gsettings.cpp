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

  openSelectorHotKey=QKeySequence(settings.value("openSelectorHotkey").toString());
  clearHistoryHotKey=QKeySequence(settings.value("clearHistoryHotkey").toString());
  pasteLastHotKey=QKeySequence(settings.value("pasteLastHotkey").toString());
  openManagerHotkey=QKeySequence(settings.value("openManagerHotkey").toString());
  openSettingsHotKey=QKeySequence(settings.value("openSettingsHotkey").toString());
  directCopyHotKey=QKeySequence(settings.value("directCopyHotkey").toString());
  closeSelectorHotkey=QKeySequence(settings.value("closeSelectorHotkey").toString());

  openSelectorHotKeyEnabled=settings.value("openSelectorHotKeyEnabled").toBool();
  clearHistoryHotKeyEnabled=settings.value("clearHistoryHotKeyEnabled").toBool();
  pasteLastHotKeyEnabled=settings.value("pasteLastHotKeyEnabled").toBool();
  openManagerHotkeyEnabled=settings.value("openManagerHotkeyEnabled").toBool();
  openSettingsHotKeyEnabled=settings.value("openSettingsHotKeyEnabled").toBool();
  directCopyHotKeyEnabled=settings.value("directCopyHotKeyEnabled").toBool();

  selectorItemBackgroundColor=settings.value("selectorItemBackgroundColor").toString();
  selectorBorderColor=settings.value("selectorBorderColor").toString();
  selectorBorderSize=settings.value("selectorBorderSize").toInt();
  selectorAnimationDuration=settings.value("selectorAnimationDuration").toInt();

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

  settings.setValue("openSelectorHotkey",openSelectorHotKey.toString(QKeySequence::NativeText));
  settings.setValue("clearHistoryHotkey",clearHistoryHotKey.toString(QKeySequence::NativeText));
  settings.setValue("pasteLastHotkey",pasteLastHotKey.toString(QKeySequence::NativeText));
  settings.setValue("openManagerHotkey",openManagerHotkey.toString(QKeySequence::NativeText));
  settings.setValue("openSettingsHotkey",openSettingsHotKey.toString(QKeySequence::NativeText));
  settings.setValue("directCopyHotkey",directCopyHotKey.toString(QKeySequence::NativeText));
  settings.setValue("closeSelectorHotkey",closeSelectorHotkey.toString(QKeySequence::NativeText));


  settings.setValue("openSelectorHotKeyEnabled",openSelectorHotKeyEnabled);
  settings.setValue("clearHistoryHotKeyEnabled",clearHistoryHotKeyEnabled);
  settings.setValue("pasteLastHotKeyEnabled",pasteLastHotKeyEnabled);
  settings.setValue("openManagerHotkeyEnabled",openManagerHotkeyEnabled);
  settings.setValue("openSettingsHotKeyEnabled",openSettingsHotKeyEnabled);
  settings.setValue("directCopyHotKeyEnabled",directCopyHotKeyEnabled);


  settings.setValue("selectorItemBackgroundColor",selectorItemBackgroundColor);
  settings.setValue("selectorBorderColor",selectorBorderColor);
  settings.setValue("selectorBorderSize",selectorBorderSize);
  settings.setValue("selectorAnimationDuration",selectorAnimationDuration);
}

bool GSettings::showInSingleLine=false;
bool GSettings::limitcharLength=false;
int GSettings::limitedCharLength=0;
int GSettings::maximumItemsInHistory=0;
bool GSettings::saveImages=false;
bool GSettings::saveUrls=false;
bool GSettings::openMinimized=false;
bool GSettings::pasteAutomaticlay=false;


QKeySequence GSettings::openSelectorHotKey=QKeySequence("Ctrl+Shift+V");
QKeySequence GSettings::clearHistoryHotKey=QKeySequence("Ctrl+Shift+Alt+C");
QKeySequence GSettings::pasteLastHotKey=QKeySequence("Ctrl+Alt+V");
QKeySequence GSettings::openManagerHotkey=QKeySequence("Ctrl+Shift+Alt+M");
QKeySequence GSettings::openSettingsHotKey=QKeySequence("Ctrl+Shift+Alt+S");
QKeySequence GSettings::directCopyHotKey=QKeySequence("Shift+Alt+C");
QKeySequence GSettings::closeSelectorHotkey=QKeySequence("Shift+Ctrl");

bool GSettings::openSelectorHotKeyEnabled=false;
bool GSettings::clearHistoryHotKeyEnabled=false;
bool GSettings::pasteLastHotKeyEnabled=false;
bool GSettings::openManagerHotkeyEnabled=false;
bool GSettings::openSettingsHotKeyEnabled=false;
bool GSettings::directCopyHotKeyEnabled=false;

QString GSettings::selectorItemBackgroundColor="";
QString GSettings::selectorBorderColor="";
int GSettings::selectorBorderSize=1;
int GSettings::selectorAnimationDuration=100;







