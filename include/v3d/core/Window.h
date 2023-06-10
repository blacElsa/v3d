//
// Created by vvin on 6/10/23.
//

#ifndef V3D_WINDOW_H
#define V3D_WINDOW_H

#include <string>
#include <cstddef>

namespace v3d {
struct WindowProps {
  std::string title;
  uint32_t width = 1600;
  uint32_t height = 900;

  explicit WindowProps(std::string t_title, uint32_t t_width = 1600, uint32_t t_height = 900) :
    title(std::move(t_title)), width(t_width), height(t_height)
  { }
};

namespace core {
class AbstractWindow {
public:
  virtual ~AbstractWindow() = default;

  virtual void Create() = 0;

  virtual void Close() = 0;
  virtual bool IsOpen() = 0;

  virtual void PollEvents() = 0;
  virtual void SwapBuffers() = 0;
};
} // core
} // v3d

#endif //V3D_WINDOW_H
