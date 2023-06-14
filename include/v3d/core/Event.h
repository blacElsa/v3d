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
  virtual void HandleEvent(const Event& e) = 0;
};

template<typename EventType>
class TypedEventHandler final : public EventHandler {
public:
  using EventCallback = std::function<void(const EventType&)>;

  explicit TypedEventHandler(EventCallback callback) :
    m_callback(std::move(callback))
  { }
  ~TypedEventHandler() final = default;

  TypedEventHandler(const TypedEventHandler&) = delete;
  TypedEventHandler& operator=(const TypedEventHandler&) = delete;

  TypedEventHandler(TypedEventHandler&&) noexcept = default;
  TypedEventHandler& operator=(TypedEventHandler&&) noexcept = default;

  void HandleEvent(const Event& e) final {
    if (const auto* typed_event = dynamic_cast<const EventType*>(&e)) {
      m_callback(*typed_event);
    }
  }
private:
  EventCallback m_callback;
};
namespace core {
class EventDispatcher {
public:
  EventDispatcher() = default;
  ~EventDispatcher() = default;

  EventDispatcher(const EventDispatcher&) = delete;
  EventDispatcher& operator=(const EventDispatcher&) = delete;

  EventDispatcher(EventDispatcher&&) = default;
  EventDispatcher& operator=(EventDispatcher&&) = default;

  template<typename EventType>
  void AddEventHandler(std::shared_ptr<EventHandler> handler) {
    m_event_handlers[&typeid(EventType)].push_back(std::move(handler));
  }

  template<typename EventType>
  void RemoveEventHandler(std::shared_ptr<EventHandler> handler) {
    if(auto it = m_event_handlers.find(&typeid(EventType)); it != m_event_handlers.end()) {
      auto&[key, handlers] = *it;
      handlers.erase(std::remove_if(handlers.begin(), handlers.end(), [&handler](const auto& stored) {
        return handler == stored;
      }), handlers.end());
    }
  }

  // TODO: Add heretical event dispatching
  void Dispatch(const Event& e) const;
private:
  // MB switch to multimap
  std::unordered_map<const std::type_info*, std::vector<std::shared_ptr<EventHandler>>> m_event_handlers;
};
} // core
} // v3d

#endif //V3D_EVENT_H
