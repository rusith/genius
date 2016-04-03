#ifndef GSETTINGS_H
#define GSETTINGS_H
#include <QSettings>
#include <QKeySequence>
#include <QFile>
class GSettings
{
public:
  GSettings();
  /**
   * @brief initialize settings (read the settings file and initialize all variables in this object)
   */
  static void initialize();
  /**
   * @brief save current settings to the settings file
   */
  static void commit();
  /**
   * @brief reset variable values to default and commit
   */
  static void resetToDefault();
  /**
   * @brief show all items in single line
   */
  static bool showInSingleLine;
  /**
   * @brief limit items text length in all views
   */
  static bool limitcharLength;
  /**
   * @brief char length of items
   */
  static int limitedCharLength;
  /**
   * @brief maximum item can store in the history
   */
  static int maximumItemsInHistory;
  /**
   * @brief save images or not
   */
  static bool saveImages;
  /**
   * @brief open Manager window minimized (default)
   */
  static bool openMinimized;
  /**
   * @brief paste selected item automaticaly
   */
  static bool pasteAutomaticlay;
  /**
   * @brief items limit of the history menu
   */
  static int historyItemLimit;
  /**
   * @brief hotkey for clear the hostory
   */
  static QKeySequence clearHistoryHotKey;
  /**
   * @brief hotkey for paste last item
   */
  static QKeySequence pasteLastHotKey;
  /**
   * @brief hotkey for open ClipboardManager
   */
  static QKeySequence openManagerHotkey;
  /**
   * @brief hotkey for open settings window
   */
  static QKeySequence openSettingsHotKey;
  /**
   * @brief clearHistoryHotKeyEnabled ?
   */
  static bool clearHistoryHotKeyEnabled;
  /**
   * @brief pasteLastHotKeyEnabled ?
   */
  static bool pasteLastHotKeyEnabled;
  /**
   * @brief openManagerHotkeyEnabled ?
   */
  static bool openManagerHotkeyEnabled;
  /**
   * @brief openSettingsHotKeyEnabled ?
   */
  static bool openSettingsHotKeyEnabled;

  static bool selectorEnabled;

  /**
   * @brief background color of items in the selector
   */
  static QString selectorItemBackgroundColor;
  /**
   * @brief text color of selector items
   */
  static QString selectorTextColor;
  /**
   * @brief border size of selectorr items
   */
  static int selectorBorderSize;
  /**
   * @brief border color of selector items
   */
  static QString selectorBorderColor;
  /**
   * @brief animation duration of the selector
   */
  static int selectorAnimationDuration;
  /**
   * @brief border radius of selector items
   */
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
   * @brief is the historyMenu hotkey enabled ?
   */
  static bool historyMenuHotkeyEnabled;

  /**
   * @brief global hotkey for open history menu inside the trayIcon
   */
  static QKeySequence historyMenuHotkey;
};

#endif // GSETTINGS_H
