#ifndef TEMPFOLDER_H
#define TEMPFOLDER_H

#include <QObject>
#include <QDir>
#include <QDebug>

/**
 * @brief This class provides interface for manage temp files of the application.
 * this class represents the temp directory. when this object constructing the temp folder will be created
 *
 */
class TempFolder : public QObject
{
  Q_OBJECT
public:

  /**
   * @brief only contructor. when creating new object the new folder will create if the folder not exists
   * @param parent QParent
   */
  explicit TempFolder(QObject *parent = 0);

  ~TempFolder();

  /**
   * @brief get path of the temp folder.if folder not exists it will created
   * @return QString path of the temp folder
   */
  QString path();

  /**
   * @brief delete the folder
   */
  void remove();

private:

  /**
   * @brief temp folder QDir object
   */
  QDir _folder;

  /**
   * @brief path to the folder (%SystemtempFolder/Genius/)
   */
  QString _path;

  /**
   * @brief create folder if it not exits
   */
  void createIfNotExists();

};

#endif // TEMPFOLDER_H
