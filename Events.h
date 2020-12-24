#ifndef EVENTS_H
#define EVENTS_H

#include "EventLib/Event.h"

class KeyPressedEvent : public Event {
    public:
        KeyPressedEvent(char _key) : key(_key) {

        }

        std::string getEventID() {
            return "KEYPRESSED";
        }

        char key;
};

class KeyHeldEvent : public Event {
    public:
        KeyHeldEvent(char _key, std::chrono::system_clock::duration _duration) : key(_key), duration(_duration) {

        }

        std::string getEventID() {
            return "KEYHELD";
        }

        char key;
        std::chrono::system_clock::duration duration;
};

class KeyReleasedEvent : public Event {
    public:
        KeyReleasedEvent(char _key) : key(_key) {

        }

        std::string getEventID() {
            return "KEYRELEASED";
        }

        char key;
};
#endif