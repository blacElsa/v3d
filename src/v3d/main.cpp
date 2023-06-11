//
// Created by vvin on 6/10/23.
//
#include <v3d/core/Event.h>
#include <v3d/core/Logger.h>
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

  auto test_handler = std::make_shared<v3d::TypedEventHandler<TestEvent>>(v3d::TypedEventHandler<TestEvent>([](const TestEvent& e) {
    V3D_APP_TRACE("Raised event {} with {}", e.name, e.num);
  }));

  auto dispatcher = v3d::core::EventDispatcher();
  dispatcher.AddEventHandler<TestEvent>(test_handler);
  dispatcher.Dispatch(TestEvent("Hello", 42));

  auto props = v3d::WindowProps("v3d-sandbox");
  auto window = v3d::GLFWWindow(std::move(props));
  window.Create();

  int count = 0;
  while (window.IsOpen()) {
    dispatcher.Dispatch(TestEvent("Tick", ++count));
    window.PollEvents();
    window.SwapBuffers();
  }

  return 0;
}