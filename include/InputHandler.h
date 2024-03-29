#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "EventQueue/EventQueue.h"
#include "RenderInclude.h"
#include <vector>
#include <chrono>
#include "TimerMap/TimerMap.h"
#include <string>

class InputHandler {
    public:
        void handleInput(GLFWwindow* window, int key, int scancode, int action, int mods, EventQueue* e, TimerMap* timerMap);
        void callRegularEvents(EventQueue* e, TimerMap* timerMap);
        void handleMouseInput(GLFWwindow* window, double xpos, double ypos, EventQueue* e, TimerMap* timerMap);
        void handleMouseButtonInput(GLFWwindow* window, int button, int action, int mods, EventQueue* e, TimerMap* timerMap);
        void handleScrollInput(GLFWwindow* window, double offsetX, double offsetY, EventQueue* e, TimerMap* timerMap);
        void setFirstMouse();
    private:
        std::vector<std::string> held = std::vector<std::string>();

        bool firstMouse = true;
        double previousXPos = 0;
        double previousYPos = 0;
};
#endif