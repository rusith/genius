
#include "fakekey.h"

#ifdef Q_OS_MAC
#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>
#endif

FakeKey::FakeKey()
{

}

void FakeKey::simulatePaste()
{
#ifdef Q_OS_LINUX
  Display *display=XOpenDisplay(NULL);
  if(display==NULL)
    return;
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
#ifdef Q_OS_MAC
  CGKeyCode inputKeyCode = kVK_ANSI_V;
  CGEventSourceRef source = CGEventSourceCreate(kCGEventSourceStateCombinedSessionState);
  CGEventRef saveCommandDown = CGEventCreateKeyboardEvent(source, inputKeyCode, 1);
  CGEventSetFlags(saveCommandDown, kCGEventFlagMaskCommand);
  CGEventRef saveCommandUp = CGEventCreateKeyboardEvent(source, inputKeyCode, 0);

  CGEventPost(kCGAnnotatedSessionEventTap, saveCommandDown);
  CGEventPost(kCGAnnotatedSessionEventTap, saveCommandUp);

  CFRelease(saveCommandUp);
  CFRelease(saveCommandDown);
  CFRelease(source);
#endif
#ifdef Q_OS_WIN
  keybd_event(VK_CONTROL,0x9d,0 , 0);
  keybd_event(0x56,0xaf,0 , 0);
  keybd_event(VK_CONTROL,0x9d, KEYEVENTF_KEYUP,0);
  keybd_event(0x56,0xaf,KEYEVENTF_KEYUP,0);
#endif
}

void FakeKey::simulateCopy()
{
#ifdef Q_OS_LINUX
  Display *display=XOpenDisplay(NULL);
  if(display==NULL)
    return; 
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
#ifdef Q_OS_MAC
  CGKeyCode inputKeyCode = kVK_ANSI_C;
  CGEventSourceRef source = CGEventSourceCreate(kCGEventSourceStateCombinedSessionState);
  CGEventRef saveCommandDown = CGEventCreateKeyboardEvent(source, inputKeyCode, 1);
  CGEventSetFlags(saveCommandDown, kCGEventFlagMaskCommand);
  CGEventRef saveCommandUp = CGEventCreateKeyboardEvent(source, inputKeyCode, 0);

  CGEventPost(kCGAnnotatedSessionEventTap, saveCommandDown);
  CGEventPost(kCGAnnotatedSessionEventTap, saveCommandUp);

  CFRelease(saveCommandUp);
  CFRelease(saveCommandDown);
  CFRelease(source);
#endif
#ifdef Q_OS_WIN
  keybd_event(VK_CONTROL,0x9D,0 , 0);
  keybd_event(0x43,0xAE,0 , 0);
  keybd_event(VK_CONTROL,0x9d, KEYEVENTF_KEYUP,0);
  keybd_event(0x43,0xAE,KEYEVENTF_KEYUP,0);
#endif
}

#ifdef Q_OS_LINUX
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



