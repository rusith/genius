#ifndef FAKEKEY_H
#define FAKEKEY_H
#ifdef __linux__
#include <X11/Xlib.h>
#include <X11/keysym.h>
#endif

class FakeKey
{
public:
  FakeKey();
  static void simulatePaste();
private:
#ifdef __linux__
  static XKeyEvent createKeyEventX11(Display *display, Window &win,Window &winRoot, bool press,int keycode, int modifiers);
#endif
};

#endif // FAKEKEY_H
