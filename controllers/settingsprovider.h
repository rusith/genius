#ifndef SETTINGSPROVIDER_H
#define SETTINGSPROVIDER_H

#include <QObject>
#include <QSettings>

class SettingsProvider : public QObject
{
  Q_OBJECT
public:
  explicit SettingsProvider();
   ~SettingsProvider();

  bool openMinimized();
  bool saveImages();
  bool saveURLs();
  bool showInSingleLine();
  int maximumItemsInHistory();
  bool limitCharLength();
  int charLength();
  bool pasteAutomaticaly();

  void openMinimized(bool b);
  void saveImages(bool value);
  void saveURLs(bool value);
  void showInSingleLine(bool value);
  void maximumItemsInHistory(int value);
  void limitCharLength(bool value);
  void charLength(int length);
  void pasteAutomaticaly(bool value);

signals:
  void settingsUpdated(SettingsProvider *provider);

public slots:

private:
  QSettings *_settings;
};

#endif // SETTINGSPROVIDER_H
