//
// Created by vvin on 6/10/23.
//
#include <glad/glad.h>

#include <v3d/core/Logger.h>
#include <v3d/platform/glfw/GLFWwindow.h>

#include <exception>

void ErrorCallback(int code, const char* desc) {
  V3D_CORE_ERROR("Error in glfw {} ({})", code, desc);
}

namespace v3d {
GLFWWindow::GLFWWindow(v3d::WindowProps property) {
  m_data.title = std::move(property.title);
  m_data.width = property.width;
  m_data.height = property.height;

  glfwSetErrorCallback(ErrorCallback);

  if (!glfwInit()) {
    V3D_CORE_CRITICAL("Failed to initialize glfw!");
    std::terminate();
  }

  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWWindow::~GLFWWindow() {
  glfwDestroyWindow(m_window);
  glfwTerminate();
}

void GLFWWindow::Create() {
  m_window = glfwCreateWindow(static_cast<int>(m_data.width), static_cast<int>(m_data.height),
                              m_data.title.c_str(), nullptr, nullptr);
  if (!m_window) {
    V3D_CORE_CRITICAL("Failed to create glfw window!");
    std::terminate();
  }

  glfwMakeContextCurrent(m_window);
  glfwSetWindowUserPointer(m_window, static_cast<void*>(this));

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    V3D_CORE_CRITICAL("Failed to initialize glad");
    std::terminate();
  }
}

void GLFWWindow::Close() {
  glfwSetWindowShouldClose(m_window, true);
}

bool GLFWWindow::IsOpen() {
  return !glfwWindowShouldClose(m_window);
}

void GLFWWindow::PollEvents() { }
void GLFWWindow::SwapBuffers() {
  glfwSwapBuffers(m_window);
}
} // v3d