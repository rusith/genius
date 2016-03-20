#include "settingsprovider.h"

SettingsProvider::SettingsProvider() : QObject()
{
   _settings=new QSettings("rusith","genius");
}

SettingsProvider::~SettingsProvider()
{
  if(_settings)
    delete _settings;
}

//______________________________________________GETTERS

bool SettingsProvider::openMinimized()
{
    return _settings->value("openMinimized").toBool();
}

bool SettingsProvider::saveImages()
{
    return _settings->value("saveImages").toBool();
}

bool SettingsProvider::saveURLs()
{
    return _settings->value("saveURLs").toBool();
}

bool SettingsProvider::showInSingleLine()
{
    return _settings->value("showInSingleLine").toBool();
}

int SettingsProvider::maximumItemsInHistory()
{
    return _settings->value("maximumItemsInHistory").toInt();
}

int SettingsProvider::charLength()
{
    if(limitCharLength())
    {
        return _settings->value("charactorLimitOfItem").toInt();
    }
    return -1;
}

bool SettingsProvider::limitCharLength()
{
    return _settings->value("limitViewCharactorLength").toBool();
}

bool SettingsProvider::pasteAutomaticaly()
{
  return _settings->value("pasteAutomaticaly").toBool();
}


//_____________________________________________________SETTERS

void SettingsProvider::openMinimized(bool b)
{
    _settings->setValue("openMinimized",b);
    emit settingsUpdated(this);
}

void SettingsProvider::saveImages(bool value)
{
    _settings->setValue("saveImages",value);
    emit settingsUpdated(this);
}


void SettingsProvider::saveURLs(bool value)
{
    _settings->setValue("saveURLs",value);
    emit settingsUpdated(this);
}

void SettingsProvider::showInSingleLine(bool value)
{
    _settings->setValue("showInSingleLine",value);
    emit settingsUpdated(this);
}

void SettingsProvider::maximumItemsInHistory(int value)
{
    _settings->setValue("maximumItemsInHistory",value);
    emit settingsUpdated(this);
}


void SettingsProvider::limitCharLength(bool value)
{
    _settings->setValue("limitViewCharactorLength",value);
    emit settingsUpdated(this);
}

void SettingsProvider::charLength(int length)
{
    if(limitCharLength())
    {
        _settings->setValue("charactorLimitOfItem",length);
        emit settingsUpdated(this);
    }
}

void SettingsProvider::pasteAutomaticaly(bool value)
{

  _settings->setValue("pasteAutomaticaly",value);
  emit settingsUpdated(this);
}
