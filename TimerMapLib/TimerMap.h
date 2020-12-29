#ifndef TIMERMAP_H
#define TIMERMAP_H

#include <chrono>
#include <map>
#include <string>

class TimerMap {
    public:
        TimerMap();

        void addTimerToMap(std::string name);

        std::chrono::system_clock::duration getTimerDuration(std::string name);

        bool resetTimer(std::string name);

        void resetAllTimers();

        std::chrono::system_clock::duration getTimerDurationAndReset(std::string name);

        bool removeTimerFromMap(std::string name);

        void removeAllTimersFromMap();

    private:
        std::map<std::string, std::chrono::system_clock::time_point> timerMap;
};
#endif