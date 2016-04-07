#ifndef RESOURCES_H
#define RESOURCES_H
#include <QFile>
#include <QString>
#include <QTemporaryDir>
#include <controllers/tempfolder.h>
class Resources
{
public:
  Resources();
  const static QString dustBin16;
  const static QString clrea16;
  const static QString settings16;
  const static QString exit16;
  const static QString logo16;
  const static QString history16;
  const static QString save16;
  const static QString cancel16;
  const static QString reset16;
  const static QString edit16;
  const static QString ok16;
  const static QString import16;
  const static QString Export16;
  const static QString colorpicker16;
  const static QString off16;
  const static QString on16;
  const static QString show16;
  const static QString up16;
  const static QString downn16;
  const static QString info16;
  const static QString pause16;
  const static QString play16;

  /**
   * @brief temp folder of the application
   */
  static TempFolder tempFolder;


};

#endif // RESOURCES_H
