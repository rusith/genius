#ifndef GSETTINGS_H
#define GSETTINGS_H
#include <QSettings>

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
};

#endif // GSETTINGS_H
