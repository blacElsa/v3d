//
// Created by vvin on 6/10/23.
//
#include <exception>

#include <glad/glad.h>
#include <v3d/core/Logger.h>
#include <v3d/core/EventType.h>

#include <v3d/platform/glfw/GLFWwindow.h>

void ErrorCallback(int code, const char* desc) {
  V3D_CORE_ERROR("Error in glfw {} ({})", code, desc);
}

namespace v3d {
GLFWWindow::GLFWWindow(v3d::WindowProps property) :
  m_data(std::move(property.title), property.width, property.height) {
  glfwSetErrorCallback(ErrorCallback);

  if (!glfwInit()) {
    V3D_CORE_CRITICAL("Failed to initialize glfw!");
    throw std::runtime_error("Failed to initialize glfw!");
  }

  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, property.gl_version_major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, property.gl_version_minor);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWWindow::~GLFWWindow() {
  if (m_window) {
      glfwDestroyWindow(m_window);
  }
  glfwTerminate();
}

void GLFWWindow::Create() {
  m_window = glfwCreateWindow(static_cast<int>(m_data.width), static_cast<int>(m_data.height),
                              m_data.title.c_str(), nullptr, nullptr);
  if (!m_window) {
    V3D_CORE_CRITICAL("Failed to create glfw window!");
    throw std::runtime_error("Failed to create glfw window!");
  }

  glfwMakeContextCurrent(m_window);
  glfwSetWindowUserPointer(m_window, static_cast<void*>(this));

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    V3D_CORE_CRITICAL("Failed to initialize glad!");
    throw std::runtime_error("Failed to initialize glad!");
  }

  SetupCallbacks();
}

void GLFWWindow::Close() {
  glfwSetWindowShouldClose(m_window, true);
}

bool GLFWWindow::IsOpen() const {
  return !glfwWindowShouldClose(m_window);
}

void GLFWWindow::BindEventDispatcher(std::shared_ptr<core::EventDispatcher> event_dispatcher) {
  assert(event_dispatcher != nullptr);
  m_event_dispatcher = event_dispatcher;
}

void GLFWWindow::PollEvents() {
  glfwPollEvents();
}

void GLFWWindow::SwapBuffers() {
  glfwSwapBuffers(m_window);
}

void GLFWWindow::SetupCallbacks() {
  glfwSetKeyCallback(m_window, [](GLFWwindow* glfw, int key, int scancode, int action, int mods) {
    auto window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(glfw));
    if (auto event_dispatcher = window->m_event_dispatcher.lock()) {
      event_dispatcher->Dispatch(KeyEvent(key, scancode, action, mods));
    }
  });

  glfwSetCursorPosCallback(m_window, [](GLFWwindow* glfw, double xpos, double ypos){
    auto window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(glfw));
    if (auto event_dispatcher = window->m_event_dispatcher.lock()) {
      event_dispatcher->Dispatch(MouseEvent(xpos, ypos));
    }
  });

  glfwSetMouseButtonCallback(m_window, [](GLFWwindow* glfw, int button, int action, int mods) {
    auto window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(glfw));
    if (auto event_dispatcher = window->m_event_dispatcher.lock()) {
      event_dispatcher->Dispatch(MouseButtonEvent(button, action, mods));
    }
  });

  glfwSetScrollCallback(m_window, [](GLFWwindow* glfw, double xoffset, double yoffset) {
    auto window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(glfw));
    if (auto event_dispatcher = window->m_event_dispatcher.lock()) {
      event_dispatcher->Dispatch(MouseScrollEvent(xoffset, yoffset));
    }
  });

  glfwSetWindowSizeCallback(m_window, [](GLFWwindow* glfw, int width, int height) {
    auto window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(glfw));
    if (auto event_dispatcher = window->m_event_dispatcher.lock()) {
      event_dispatcher->Dispatch(WindowSizeEvent(width, height));
    }
  });

  glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* glfw, int width, int height) {
    auto window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(glfw));
    if (auto event_dispatcher = window->m_event_dispatcher.lock()) {
      event_dispatcher->Dispatch(FramebufferSizeEvent(width, height));
    }
  });

  glfwSetWindowCloseCallback(m_window, [](GLFWwindow* glfw) {
    auto window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(glfw));
    if (auto event_dispatcher = window->m_event_dispatcher.lock()) {
      event_dispatcher->Dispatch(WindowCloseEvent());
    }
  });

  glfwSetWindowFocusCallback(m_window, [](GLFWwindow* glfw, int focus) {
    auto window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(glfw));
    if (auto event_dispatcher = window->m_event_dispatcher.lock()) {
      event_dispatcher->Dispatch(WindowFocusEvent(focus));
    }
  });
}
} // v3d