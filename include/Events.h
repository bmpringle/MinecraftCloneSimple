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

class MouseMovedOffsetEvent : public Event {
    public:
        MouseMovedOffsetEvent(double _xOffset, double _yOffset) : xOffset(_xOffset), yOffset(_yOffset) {

        }

        std::string getEventID() {
            return "MOUSEMOVEDOFFSET";
        }

        double xOffset;
        double yOffset;
};

class MousePosEvent : public Event {
    public:
        MousePosEvent(double x, double y) : x(x), y(y) {

        }

        std::string getEventID() {
            return "MOUSEPOS";
        }

        double x;
        double y;
};

class LeftMouseButtonPressedEvent : public Event {
    public:
        LeftMouseButtonPressedEvent() {

        }

        std::string getEventID() {
            return "LEFT_CLICK";
        }
};

class RightMouseButtonPressedEvent : public Event {
    public:
        RightMouseButtonPressedEvent() {

        }

        std::string getEventID() {
            return "RIGHT_CLICK";
        }
};

class ScrollEvent : public Event {
    public:
        ScrollEvent(int x, int y) : offsetX(x), offsetY(y) {

        }

        std::string getEventID() {
            return "SCROLL";
        }
    
        int offsetX;
        int offsetY;
};
#endif