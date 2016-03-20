#ifndef GSETTINGS_H
#define GSETTINGS_H
#include <QSettings>
#include <QKeySequence>

class GSettings
{
public:
  GSettings();
  static void initialize();
  static void commit();
  static bool showInSingleLine;
  static bool limitcharLength;
  static int limitedCharLength;
  static int maximumItemsInHistory;
  static bool saveImages;
  static bool saveUrls;
  static bool openMinimized;
  static bool pasteAutomaticlay;
  static QKeySequence openSelectorHotKey;
  static QKeySequence clearHistoryHotKey;
  static QKeySequence pasteLastHotKey;
  static QKeySequence openManagerHotkey;
  static QKeySequence openSettingsHotKey;
  static QKeySequence directCopyHotKey;


  static bool openSelectorHotKeyEnabled;
  static bool clearHistoryHotKeyEnabled;
  static bool pasteLastHotKeyEnabled;
  static bool openManagerHotkeyEnabled;
  static bool openSettingsHotKeyEnabled;
  static bool directCopyHotKeyEnabled;


};

#endif // GSETTINGS_H
