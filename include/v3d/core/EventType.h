//
// Created by vvin on 6/11/23.
//

#ifndef V3D_EVENTTYPE_H
#define V3D_EVENTTYPE_H

#include <v3d/core/Event.h>

namespace v3d {
// TODO: Add event heretic
class KeyEvent : public Event {
public:
  KeyEvent(int t_key, int t_scancode, int t_action, int t_mods) :
    key(t_key), scancode(t_scancode), action(t_action), mods(t_mods)
  { }

  int key;
  int scancode;
  int action;
  int mods;
};

class MouseEvent : public Event {
public:
  MouseEvent(double t_x, double t_y) :
    x(t_x), y(t_y)
  { }

  double x, y;
};

class MouseButtonEvent : public Event {
public:
  MouseButtonEvent(int t_button, int t_action, int t_mods) :
    button(t_button), action(t_action), mods(t_mods)
  { }

  int button;
  int action;
  int mods;
};

class MouseScrollEvent : public Event {
public:
  MouseScrollEvent(double t_x_offset, double t_y_offset) :
    x_offset(t_x_offset), y_offset(t_y_offset)
  { }

  double x_offset, y_offset;
};

class WindowSizeEvent : public Event {
public:
  WindowSizeEvent(int t_width, int t_height) :
    width(t_width), height(t_height)
  { }

  int width, height;
};

class FramebufferSizeEvent : public Event {
public:
  FramebufferSizeEvent(int t_width, int t_height) :
    width(t_width), height(t_height)
  { }

  int width, height;
};

class WindowCloseEvent : public Event { };

class WindowFocusEvent : public Event {
public:
  explicit WindowFocusEvent(bool t_focus) :
    focus(t_focus)
  { }

  bool focus;
};
}

#endif //V3D_EVENTTYPE_H
