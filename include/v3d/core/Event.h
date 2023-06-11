//
// Created by vvin on 6/10/23.
//

#ifndef V3D_EVENT_H
#define V3D_EVENT_H

#include <memory>
#include <vector>
#include <typeinfo>
#include <algorithm>
#include <functional>
#include <unordered_map>

namespace v3d {
class Event {
public:
  virtual ~Event() = default;
};

class EventHandler {
public:
  virtual ~EventHandler() = default;
  virtual void operator()(const Event& e) = 0;
};

template<typename EventType, typename HandlerType = EventHandler>
class TypedEventHandler : public HandlerType {
public:
  using EventCallback = std::function<void(const EventType&)>;

  explicit TypedEventHandler(EventCallback callback) :
    m_callback(std::move(callback))
  { }

  void operator()(const Event& e) override {
    if (const auto* typed_event = dynamic_cast<const EventType*>(&e)) {
      m_callback(*typed_event);
    }
  }
private:
  EventCallback m_callback;
};
namespace core {
template<typename HandlerType = EventHandler>
class EventDispatcher {
public:
  template<typename EventType>
  void AddEventHandler(std::shared_ptr<HandlerType> handler) {
    m_event_handlers[&typeid(EventType)].push_back(std::move(handler));
  }

  template<typename EventType>
  void RemoveEventHandler(std::shared_ptr<HandlerType> handler) {
    if(auto it = m_event_handlers.find(&typeid(EventType)); it != m_event_handlers.end()) {
      auto&[key, handlers] = *it;
      handlers.erase(std::remove_if(handlers.begin(), handlers.end(), [&handler](const auto& stored) {
        return handler == stored;
      }), handlers.end());
    }
  }

  void Dispatch(const Event& e) const {
    if(auto it = m_event_handlers.find(&typeid(e)); it != m_event_handlers.end()) {
      auto&[key, handlers] = *it;
      for (const auto& handler : handlers) {
        (*handler)(e);
      }
    }
  }
private:
  // MB switch to multimap
  std::unordered_map<const std::type_info*, std::vector<std::shared_ptr<HandlerType>>> m_event_handlers;
};
} // core
} // v3d

#endif //V3D_EVENT_H
