//
// Created by vvin on 6/10/23.
//
#include <v3d/core/Event.h>
#include <v3d/core/Logger.h>
#include <v3d/core/EventType.h>
#include <v3d/platform/glfw/GLFWwindow.h>

class TestEvent : public v3d::Event {
public:
  TestEvent(std::string t_name, int t_num) :
    name(std::move(t_name)), num(t_num) { }

  std::string name;
  int num;
};

int main() {
  v3d::core::Log::Init();
  V3D_APP_INFO("Create window");

  auto test_handler = std::make_shared<v3d::TypedEventHandler<TestEvent>>([](const TestEvent& e) {
    V3D_APP_TRACE("Raised event {} with {}", e.name, e.num);
  });

  auto dispatcher = std::make_shared<v3d::core::EventDispatcher>(v3d::core::EventDispatcher());
  dispatcher->AddEventHandler<TestEvent>(test_handler);
  dispatcher->Dispatch(TestEvent("Hello", 42));

  auto props = v3d::WindowProps("v3d-sandbox");
  auto window = v3d::MakeWindow<v3d::GLFWWindow>(std::move(props));

  window->BindEventDispatcher(dispatcher);

  auto key_handler = std::make_shared<v3d::TypedEventHandler<v3d::KeyEvent>>([](const v3d::KeyEvent& e) {
    std::string action;
    switch (e.action) {
      case GLFW_PRESS:
        action = "Pressed";
        break;
      case GLFW_RELEASE:
        action = "Released";
        break;
      case GLFW_REPEAT:
        action = "Repeat";
        break;
      default:
        return;
    }
    V3D_APP_TRACE("Key {}: {}", e.key, action);
  });

  auto mouse_handler = std::make_shared<v3d::TypedEventHandler<v3d::MouseEvent>>([](const v3d::MouseEvent& e) {
    V3D_APP_TRACE("Mouse: ({};{})", e.x, e.y);
  });

  dispatcher->AddEventHandler<v3d::KeyEvent>(key_handler);
  dispatcher->AddEventHandler<v3d::MouseEvent>(mouse_handler);

  while (window->IsOpen()) {
    window->PollEvents();
    window->SwapBuffers();
  }

  return 0;
}