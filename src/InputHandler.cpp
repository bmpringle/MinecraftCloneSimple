#include "InputHandler.h"
#include "Events.h"
#include <memory>
#include "GLFWKeyCodeConverter.h"
#include <iostream>
#include <string>

void safeRemoveFromVector(std::vector<char>* vector, char remove) {
    for(int i = 0; i < vector->size(); ++i) {
        if(vector->at(i) == remove) {
            vector->erase(vector->begin() + i);
        }
    }
}

void safeAddToVector(std::vector<char>* vector, char add) {
    safeRemoveFromVector(vector, add);
    vector->push_back(add);
}

void InputHandler::handleInput(GLFWwindow* window, int key, int scancode, int action, int mods, EventQueue* e, TimerMap* timerMap) {
    std::optional<char> cOptional = fromKeyCode(key);
    if(!cOptional.has_value()) {
        return;
    }

    char c = cOptional.value();

    switch(action) {
        case GLFW_PRESS:
            e->callEvent(std::shared_ptr<KeyPressedEvent>(new KeyPressedEvent(c)));
            if(timerMap->getTimerDuration(std::string(1, c)).count() == 0) {
                timerMap->addTimerToMap(std::string(1, c));
            }
            safeAddToVector(&held, c);
            break;
        case GLFW_RELEASE:
            e->callEvent(std::shared_ptr<KeyReleasedEvent>(new KeyReleasedEvent(c)));
            timerMap->removeTimerFromMap(std::string(1, c));
            safeRemoveFromVector(&held, c);
            break;
        default:
            break;
    }

    for(int i = 0; i < held.size(); ++i) {
        std::optional<int> intOptional = toKeyCode(held.at(i));
        if(intOptional.has_value()) {
            if(glfwGetKey(window, intOptional.value()) == GLFW_RELEASE) {
                held.erase(held.begin() + i);
            }
        }
    }
}

void InputHandler::handleMouseInput(GLFWwindow* window, double xpos, double ypos, EventQueue* e, TimerMap* timerMap) {
    if(firstMouse) {
        previousXPos = xpos;
        previousYPos = ypos;
        firstMouse = false;
        return;
    }

    double sensitivity = 0.1;
    double xOffset = sensitivity*(xpos - previousXPos);
    double yOffset = sensitivity*(ypos - previousYPos);
    if(xOffset != 0 || yOffset != 0) {
        e->callEvent(std::shared_ptr<MouseMovedEvent>(new MouseMovedEvent(xOffset, yOffset)));
    }

    previousXPos = xpos;
    previousYPos = ypos;
}

void InputHandler::setFirstMouse() {
    firstMouse = true;
}

void InputHandler::callRegularEvents(EventQueue* e, TimerMap* timerMap) {
    for(int i = 0; i < held.size(); ++i) {
        std::chrono::system_clock::duration duration = timerMap->getTimerDurationAndReset(std::string(1, held.at(i)));
        e->callEvent(std::shared_ptr<KeyHeldEvent>(new KeyHeldEvent(held.at(i), duration)));
    }
}