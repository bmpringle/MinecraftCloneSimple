#ifndef EVENTS_H
#define EVENTS_H

#include "EventLib/Event.h"
#include <chrono>
#include <string>

class KeyPressedEvent : public Event {
    public:
        KeyPressedEvent(std::string _key) : key(_key) {

        }

        std::string getEventID() {
            return "KEYPRESSED";
        }

        std::string key;
};

class KeyHeldEvent : public Event {
    public:
        KeyHeldEvent(std::string _key, std::chrono::system_clock::duration _duration) : key(_key), duration(_duration) {

        }

        std::string getEventID() {
            return "KEYHELD";
        }

        std::string key;
        std::chrono::system_clock::duration duration;
};

class KeyReleasedEvent : public Event {
    public:
        KeyReleasedEvent(std::string _key) : key(_key) {

        }

        std::string getEventID() {
            return "KEYRELEASED";
        }

        std::string key;
};

class MouseMovedEvent : public Event {
    public:
        MouseMovedEvent(double _xOffset, double _yOffset) : xOffset(_xOffset), yOffset(_yOffset) {

        }

        std::string getEventID() {
            return "MOUSEMOVED";
        }

        double xOffset;
        double yOffset;
};

class LeftMouseButtonPressedEvent : public Event {
    public:
        LeftMouseButtonPressedEvent() {

        }

        std::string getEventID() {
            return "LEFTMOUSEBUTTONPRESSED";
        }
};

class RightMouseButtonPressedEvent : public Event {
    public:
        RightMouseButtonPressedEvent() {

        }

        std::string getEventID() {
            return "RIGHTMOUSEBUTTONPRESSED";
        }
};
#endif