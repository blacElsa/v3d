//
// Created by vvin on 6/10/23.
//

#ifndef V3D_GLFWWINDOW_H
#define V3D_GLFWWINDOW_H

#include <GLFW/glfw3.h>

#include <v3d/core/Window.h>

namespace v3d {
class GLFWWindow : public core::AbstractWindow {
  struct WindowData {
    std::string title;
    uint32_t width = 1600;
    uint32_t height = 900;
  };
public:
  explicit GLFWWindow(WindowProps property);
  ~GLFWWindow() override;

  void Create() override;

  void Close() final;
  bool IsOpen() final;

  void BindEventDispatcher(std::shared_ptr<core::EventDispatcher>) override;
  void PollEvents() override;

  void SwapBuffers() override;
private:
  void SetupCallbacks();
private:
  ::GLFWwindow* m_window = nullptr;
  std::weak_ptr<core::EventDispatcher> m_event_dispatcher;
  WindowData m_data;
};
}

#endif //V3D_GLFWWINDOW_H
