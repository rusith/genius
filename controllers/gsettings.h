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
  static int historyItemLimit;


  static QKeySequence clearHistoryHotKey;
  static QKeySequence pasteLastHotKey;
  static QKeySequence openManagerHotkey;
  static QKeySequence openSettingsHotKey;
  static QKeySequence directCopyHotKey;

  static bool clearHistoryHotKeyEnabled;
  static bool pasteLastHotKeyEnabled;
  static bool openManagerHotkeyEnabled;
  static bool openSettingsHotKeyEnabled;
  static bool directCopyHotKeyEnabled;

  static QString selectorItemBackgroundColor;
  static QString selectorTextColor;
  static int selectorBorderSize;
  static QString selectorBorderColor;
  static int selectorAnimationDuration;
  static int selectorBorderRadius;

  /**
   * @brief maximum image width.images larger than this  are saved in a file
   */
  static int maximumImageWidth;

  /**
   * @brief maximum image width.images larger than this are saved in a file
   */
  static int maximumImageHight;

  /**
   * @brief  maximum text length an item can save in memory
   */
  static int inMemoryTextLength;

  /**
   * @brief is the historyMenu hotkey enabled ?
   */
  static bool historyMenuHotkeyEnabled;

  /**
   * @brief global hotkey for open history menu inside the trayIcon
   */
  static QKeySequence historyMenuHotkey;


};

#endif // GSETTINGS_H
