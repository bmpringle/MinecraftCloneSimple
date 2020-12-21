#ifndef LISTENER_H
#define LISTENER_H

#include <vector>
#include "Event.h"
#include <memory>

class Listener {
    public:
        virtual void listenTo(std::shared_ptr<Event> e) = 0;
};
#endif