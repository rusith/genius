#include "menu.h"

Menu::Menu(QWidget *parent) :QMenu(parent)
{

}

void Menu::keyPressEvent(QKeyEvent *event)
{
  emit keyPressed(event->key());
  QMenu::keyPressEvent(event);
}

