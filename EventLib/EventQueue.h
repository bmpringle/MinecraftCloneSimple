#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include <string>
#include <vector>
#include "Listener.h"

class EventQueue {
    public:
        EventQueue();

        void addEventListener(std::shared_ptr<Listener> l);

        void removeEventListener(std::shared_ptr<Listener> l);

        void callEvent(std::shared_ptr<Event> e);
    private:
        std::vector<std::shared_ptr<Listener>> listenerArray;
};
#endif