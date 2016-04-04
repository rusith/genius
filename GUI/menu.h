#ifndef MENU_H
#define MENU_H
#include <QMenu>
#include <QDebug>
#include <QKeyEvent>
class Menu : public QMenu
{
  Q_OBJECT
public:
  Menu(QWidget *parent);
  void keyPressEvent(QKeyEvent *event);
signals:
  void keyPressed(int key);
};

#endif // MENU_H
