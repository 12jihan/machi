#include "../include/EventManager.hpp"

void EventManager::subscribe(const EventHandler& handler) {
  m_subscribers.push_back(handler);
}

void EventManager::postEvent(const Event& event) {
  m_eventQueue.push_back(event);
}

void EventManager::dispatchEvents() {
  // For all queued event
  for (const auto& event : m_eventQueue) {
    // For each event notify each subscriber
    for (const auto& subscriber : m_subscribers) {
      subscriber(event);
    }
  }

  // Clear for the next frame
  m_eventQueue.clear();
}

void EventManager::clearQueue() {
  m_eventQueue.clear();
}

int EventManager::getEventLength() {
  return m_eventQueue.size();
}

int EventManager::getSubcriberLength() {
  return m_subscribers.size();
}
