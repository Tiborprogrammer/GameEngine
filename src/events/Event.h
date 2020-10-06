//
// Created by Tibor Varga on 03/09/2020.
//

#ifndef GAMEENGINE_EVENT_H
#define GAMEENGINE_EVENT_H

#include <string>

enum class EventType {
    None = 0,
    WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
    AppTick, AppUpdate, AppRender,
    KeyPressed, KeyReleased,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

#define BIT(i) (1 << i)

enum EventCategory {
    None = 0,
    EventCategoryApplication    = BIT(0),
    EventCategoryInput          = BIT(1),
    EventCategoryKeyboard       = BIT(2),
    EventCategoryMouse          = BIT(3),
    EventCategoryMouseButton    = BIT(4)
};

class Event {
public:
    EventType eventType;
    EventCategory eventCategory;
    bool handled = false;

    bool isCategory(EventCategory category) { return (category & eventCategory); }
    std::string GetName()  {
        if (eventType == EventType::None) return "NONE";
        if (eventType >= EventType::WindowClose and eventType <= EventType::WindowMoved) return "WINDOW EVENT";
        if (eventType >= EventType::AppTick and eventType <= EventType::AppRender) return "APP EVENT";
        if (eventType >= EventType::KeyPressed and eventType <= EventType::KeyReleased) return "KEYBOARD EVENT";
        if (eventType >= EventType::MouseButtonPressed and eventType <= EventType::MouseScrolled) return "MOUSE EVENT";
    };
};

#endif //GAMEENGINE_EVENT_H