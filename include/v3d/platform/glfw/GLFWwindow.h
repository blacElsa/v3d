//
// Created by vvin on 6/10/23.
//

#ifndef V3D_GLFWWINDOW_H
#define V3D_GLFWWINDOW_H

#include <v3d/core/Window.h>
#include <GLFW/glfw3.h>

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

  void PollEvents() override;
  void SwapBuffers() override;
private:
  ::GLFWwindow* m_window = nullptr;
  WindowData m_data;
};
}

#endif //V3D_GLFWWINDOW_H
