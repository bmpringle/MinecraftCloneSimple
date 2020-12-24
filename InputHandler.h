#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "EventLib/EventQueue.h"
#include "RenderInclude.h"
#include <vector>
#include <chrono>

struct CharTime {
    public:
        CharTime(char _c) : c(_c), lastTimeCalled(std::chrono::system_clock::now()) {

        }

        char c;
        std::chrono::system_clock::time_point lastTimeCalled;
};

class InputHandler {
    public:
        void handleInput(GLFWwindow* window, int key, int scancode, int action, int mods, EventQueue* e);
        void callRegularEvents(EventQueue* e);
    private:
        std::vector<CharTime> held = std::vector<CharTime>();
};
#endif