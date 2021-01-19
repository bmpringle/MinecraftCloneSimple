#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <vector>
class Event {
    public:
        virtual std::string getEventID() = 0;

        virtual ~Event() = default;
};
#endif