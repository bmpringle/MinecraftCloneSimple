#include "InputHandler.h"
#include "Events.h"
#include <memory>
#include "GLFWKeyCodeConverter.h"
#include <iostream>

void safeRemoveFromVector(std::vector<CharTime>* vector, CharTime remove) {
    for(int i = 0; i < vector->size(); ++i) {
        if(vector->at(i).c == remove.c) {
            vector->erase(vector->begin() + i - 1);
        }
    }
}

void safeAddToVector(std::vector<CharTime>* vector, CharTime add) {
    safeRemoveFromVector(vector, add);
    vector->push_back(add);
}

void InputHandler::handleInput(GLFWwindow* window, int key, int scancode, int action, int mods, EventQueue* e) {
    std::optional<char> cOptional = fromKeyCode(key);
    if(!cOptional.has_value()) {
        return;
    }

    char c = cOptional.value();

    switch(action) {
        case GLFW_PRESS:
            e->callEvent(std::shared_ptr<KeyPressedEvent>(new KeyPressedEvent(c)));
            safeAddToVector(&held, CharTime(c));
            break;
        case GLFW_RELEASE:
            e->callEvent(std::shared_ptr<KeyReleasedEvent>(new KeyReleasedEvent(c)));
            safeRemoveFromVector(&held, CharTime(c));
            break;
        default:
            break;
    }

    for(int i = 0; i < held.size(); ++i) {
        CharTime keyHeld = held.at(i);
        std::optional<int> intOptional = toKeyCode(keyHeld.c);
        if(intOptional.has_value()) {
            if(glfwGetKey(window, intOptional.value()) == GLFW_RELEASE) {
                held.erase(held.begin() + i - 1);
            }
        }
    }
}

void InputHandler::callRegularEvents(EventQueue* e) {
    for(int i = 0; i < held.size(); ++i) {
        CharTime keyHeld = held.at(i);
        std::chrono::system_clock::duration duration = std::chrono::system_clock::now() - keyHeld.lastTimeCalled;
        e->callEvent(std::shared_ptr<KeyHeldEvent>(new KeyHeldEvent(keyHeld.c, duration)));
        held.at(i).lastTimeCalled = std::chrono::system_clock::now();
    }
}