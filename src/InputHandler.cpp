#include "InputHandler.h"
#include "Events.h"
#include <memory>
#include "GLFWKeyCodeConverter.h"
#include <iostream>
#include <string>

void safeRemoveFromVector(std::vector<std::string>* vector, std::string remove) {
    for(int i = 0; i < vector->size(); ++i) {
        if(vector->at(i) == remove) {
            vector->erase(vector->begin() + i);
        }
    }
}

void safeAddToVector(std::vector<std::string>* vector, std::string add) {
    safeRemoveFromVector(vector, add);
    vector->push_back(add);
}

void InputHandler::handleInput(GLFWwindow* window, int key, int scancode, int action, int mods, EventQueue* e, TimerMap* timerMap) {
    std::optional<std::string> cOptional = fromKeyCode(key);

    if(!cOptional.has_value()) {
        return;
    }

    std::string c = cOptional.value();

    switch(action) {
        case GLFW_PRESS:
            e->callEvent(std::shared_ptr<KeyPressedEvent>(new KeyPressedEvent(c)));
            if(timerMap->getTimerDuration(c).count() == 0) {
                timerMap->addTimerToMap(c);
            }
            safeAddToVector(&held, c);
            break;
        case GLFW_RELEASE:
            e->callEvent(std::shared_ptr<KeyReleasedEvent>(new KeyReleasedEvent(c)));
            timerMap->removeTimerFromMap(c);
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
        e->callEvent(std::shared_ptr<MouseMovedOffsetEvent>(new MouseMovedOffsetEvent(xOffset, yOffset)));
    }

    previousXPos = xpos;
    previousYPos = ypos;

    e->callEvent(std::shared_ptr<MousePosEvent>(new MousePosEvent(xpos, ypos)));
}

void InputHandler::handleMouseButtonInput(GLFWwindow* window, int button, int action, int mods, EventQueue* e, TimerMap* timerMap) {
    switch(button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            switch(action) {
                case GLFW_PRESS:
                    e->callEvent(std::shared_ptr<LeftMouseButtonPressedEvent>(new LeftMouseButtonPressedEvent()));
            }
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            switch(action) {
                case GLFW_PRESS:
                    e->callEvent(std::shared_ptr<RightMouseButtonPressedEvent>(new RightMouseButtonPressedEvent()));
            }
            break;
    }
}

void InputHandler::setFirstMouse() {
    firstMouse = true;
}

void InputHandler::callRegularEvents(EventQueue* e, TimerMap* timerMap) {
    for(int i = 0; i < held.size(); ++i) {
        std::chrono::system_clock::duration duration = std::chrono::duration_cast<std::chrono::milliseconds>(timerMap->getTimerDurationAndReset(held.at(i)));
        e->callEvent(std::shared_ptr<KeyHeldEvent>(new KeyHeldEvent(held.at(i), duration)));
    }
}

void InputHandler::handleScrollInput(GLFWwindow* window, double offsetX, double offsetY, EventQueue* e, TimerMap* timerMap) {
    double sensitivity = 2;
    e->callEvent(std::shared_ptr<ScrollEvent>(new ScrollEvent(sensitivity * offsetX, sensitivity * offsetY)));
}