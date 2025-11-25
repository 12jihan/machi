#pragma once
#include <vector>
#include <functional>
#include <string>

enum class EventType {
  WindowResize,
  WindowClose,
  KeyPress,
  KeyRelease,
  MousePress,
  MouseRelease,
  MouseMove,
  MouseScroll,
  EngineShutdown
};

struct Event {
  EventType type;
  std::string eventName;

  // Union to hold different event data efficiently
  union {
    struct {
      int width, height;
    } resize;
    struct {
      int key, scancode, mods;
    } keyboard;
    struct {
      int button, mods;
    } mouse;
    struct {
      double x, y;
    } mousePos;
    struct {
      double xOffset, yOffset;
    } scroll;
  } data;

  double timestamp;  // When the event occurred
};

// Defining the Callback Type
using EventHandler = std::function<void(const Event&)>;

class EventManager {
private:
  // List of subscribers & Queue of events waiting to be processed:
  std::vector<EventHandler> m_subscribers;
  std::vector<Event> m_eventQueue;

public:
  EventManager() = default;
  ~EventManager() = default;

  // Subscibe a function to listen for events
  void subscribe(const EventHandler& handler);

  // Add an event to the queue (to be processed next frame)
  void postEvent(const Event& event);

  // Dispatch all queued events to subscribers immediately
  void dispatchEvents();

  // Utility: Clear all events without processing (good for resets)
  void clearQueue();

  // Getters
  int getEventLength();

  int getSubcriberLength();
};
