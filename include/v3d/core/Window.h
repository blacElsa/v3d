//
// Created by vvin on 6/10/23.
//

#ifndef V3D_WINDOW_H
#define V3D_WINDOW_H

#include <memory>
#include <string>
#include <cstddef>
#include <concepts>

#include <v3d/core/Event.h>

namespace v3d {
struct WindowProps {
  std::string title;
  uint32_t width = 1600;
  uint32_t height = 900;

  int gl_version_major = 3;
  int gl_version_minor = 3;

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
  [[nodiscard]]
  virtual bool IsOpen() const = 0;

  virtual void BindEventDispatcher(std::shared_ptr<EventDispatcher>) = 0;
  virtual void PollEvents() = 0;

  virtual void SwapBuffers() = 0;
};
} // core

template<std::derived_from<core::AbstractWindow> PlatformType>
std::unique_ptr<core::AbstractWindow> MakeWindow(WindowProps props) {
  auto window = std::make_unique<PlatformType>(std::move(props));
  window->Create();
  return window;
}
} // v3d

#endif //V3D_WINDOW_H
