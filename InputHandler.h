#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "EventLib/EventQueue.h"
#include "RenderInclude.h"
#include <vector>
#include <chrono>
#include "TimerMapLib/TimerMap.h"

class InputHandler {
    public:
        void handleInput(GLFWwindow* window, int key, int scancode, int action, int mods, EventQueue* e, TimerMap* timerMap);
        void callRegularEvents(EventQueue* e, TimerMap* timerMap);
    private:
        std::vector<char> held = std::vector<char>();
};
#endif