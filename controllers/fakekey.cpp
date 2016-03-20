
#include "fakekey.h"

FakeKey::FakeKey()
{

}

void FakeKey::simulatePaste()
{
#ifdef __linux__
  Display *display=XOpenDisplay(NULL);
  if(display==NULL)
  {
    return;
  }
  Window winRoot=XDefaultRootWindow(display);
  Window winFocus;
  int revert;
  XGetInputFocus(display, &winFocus, &revert);
  XKeyEvent  PressEventV = createKeyEventX11(display, winFocus, winRoot, true, XK_V,ControlMask);
  XKeyEvent  ReleasEventV = createKeyEventX11(display, winFocus, winRoot, false, XK_V,ControlMask);
  XSelectInput(display,winFocus,KeyPressMask|KeyReleaseMask);
  XSendEvent(PressEventV.display, PressEventV.window, True, KeyPressMask, (XEvent *)&PressEventV);
  XSendEvent(ReleasEventV.display, ReleasEventV.window, True, KeyReleaseMask, (XEvent *)&ReleasEventV);
  XCloseDisplay(display);
#endif
}

void FakeKey::simulateCopy()
{
#ifdef __linux__
  Display *display=XOpenDisplay(NULL);
  if(display==NULL)
  {
    return;
  }
  Window winRoot=XDefaultRootWindow(display);
  Window winFocus;
  int revert;
  XGetInputFocus(display, &winFocus, &revert);
  XKeyEvent  PressEventV = createKeyEventX11(display, winFocus, winRoot, true, XK_C,ControlMask);
  XKeyEvent  ReleasEventV = createKeyEventX11(display, winFocus, winRoot, false, XK_C,ControlMask);
  XSelectInput(display,winFocus,KeyPressMask|KeyReleaseMask);
  XSendEvent(PressEventV.display, PressEventV.window, True, KeyPressMask, (XEvent *)&PressEventV);
  XSendEvent(ReleasEventV.display, ReleasEventV.window, True, KeyReleaseMask, (XEvent *)&ReleasEventV);
  XCloseDisplay(display);
#endif
}

#ifdef __linux__
XKeyEvent FakeKey::createKeyEventX11(Display *display, Window &win,Window &winRoot, bool press,int keycode, int modifiers)
{

  XKeyEvent event;
  event.display=display;
  event.window=win;
  event.root        = winRoot;
  event.subwindow   = None;
  event.time        = CurrentTime;
  event.x           = 1;
  event.y           = 1;
  event.x_root      = 1;
  event.y_root      = 1;
  event.same_screen = True;
  event.keycode     = XKeysymToKeycode(display, keycode);
  event.state       = modifiers;
  if(press)
  {
    event.type=KeyPress;
  }
  else
  {
    event.type=KeyRelease;
  }

  return event;
}
#endif
