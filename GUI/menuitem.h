#ifndef MENUITEM_H
#define MENUITEM_H
#include <QAction>

class MenuItem :public QAction
{
  Q_OBJECT
public:
  explicit MenuItem(QObject *parent);

  bool indexed();
  void indexed(bool inde);
  void index(const int &inde);
  const int &index();
  void setTxt(const QString &text);
private:
  bool indexed_;
  int index_;
  int indexLength_;
};

#endif // MENUITEM_H
