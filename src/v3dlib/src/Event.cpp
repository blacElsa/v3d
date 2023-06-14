//
// Created by vvin on 6/11/23.
//

#include <v3d/core/Event.h>

namespace v3d::core {
void EventDispatcher::Dispatch(const v3d::Event& e) const {
  if(auto it = m_event_handlers.find(&typeid(e)); it != m_event_handlers.end()) {
    auto&[key, handlers] = *it;
    for (const auto& handler : handlers) {
      handler->HandleEvent(e);
    }
  }
}
} // v3d::core
