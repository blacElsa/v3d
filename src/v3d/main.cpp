//
// Created by vvin on 6/10/23.
//
#include <v3d/core/Logger.h>
#include <v3d/platform/glfw/GLFWwindow.h>

int main() {
  v3d::core::Log::Init();
  v3d::WindowProps props("v3d-sandbox");
  auto window = v3d::GLFWWindow(props);
  window.Create();

  while (window.IsOpen()) {
    window.PollEvents();
    window.SwapBuffers();
  }

  return 0;
}