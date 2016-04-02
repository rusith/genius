#ifndef FAKEKEY_H
#define FAKEKEY_H
#include <QtCore>
#ifdef Q_OS_LINUX
  #include <X11/Xlib.h>
  #include <X11/keysym.h>
#endif
#ifdef Q_OS_WIN
    #include <Windows.h>
#endif

class FakeKey
{
public:
  FakeKey();
  static void simulatePaste();
  static void simulateCopy();
private:
#ifdef Q_OS_LINUX
  static XKeyEvent createKeyEventX11(Display *display, Window &win,Window &winRoot, bool press,int keycode, int modifiers);
#endif


#
};

#endif // FAKEKEY_H
