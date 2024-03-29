#include "InputHandler.h"
#include "Events.h"
#include <memory>
#include "GLFWKeyCodeConverter.h"
#include <iostream>
#include <string>

void InputHandler::handleInput(GLFWwindow* window, int key, int scancode, int action, int mods, EventQueue* e, TimerMap* timerMap) { 
    std::optional<std::string> cOptional = fromKeyCode(key);

    if(!cOptional.has_value()) {
        return;
    }

    std::string c = cOptional.value();

    auto it = std::find(held.begin(), held.end(), c);

    switch(action) {
        case GLFW_PRESS:
            e->callEvent(std::make_shared<KeyPressedEvent>(c));
            if(timerMap->getTimerDuration(c).count() == 0) {
                timerMap->addTimerToMap(c);
            }

            if(it != held.end()) {
                held.erase(it);
            }
            held.push_back(c);
            break;
        case GLFW_RELEASE:
            e->callEvent(std::shared_ptr<KeyReleasedEvent>(std::make_shared<KeyReleasedEvent>(c)));
            timerMap->removeTimerFromMap(c);
            if(it != held.end()) {
                held.erase(it);
            }
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
        e->callEvent(std::shared_ptr<MouseMovedOffsetEvent>(std::make_shared<MouseMovedOffsetEvent>(xOffset, yOffset)));
    }

    previousXPos = xpos;
    previousYPos = ypos;

    e->callEvent(std::shared_ptr<MousePosEvent>(std::make_shared<MousePosEvent>(xpos, ypos)));
}

void InputHandler::handleMouseButtonInput(GLFWwindow* window, int button, int action, int mods, EventQueue* e, TimerMap* timerMap) {
    switch(button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            switch(action) {
                case GLFW_PRESS:
                    e->callEvent(std::shared_ptr<LeftMouseButtonPressedEvent>(std::make_shared<LeftMouseButtonPressedEvent>()));
            }
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            switch(action) {
                case GLFW_PRESS:
                    e->callEvent(std::shared_ptr<RightMouseButtonPressedEvent>(std::make_shared<RightMouseButtonPressedEvent>()));
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
        e->callEvent(std::shared_ptr<KeyHeldEvent>(std::make_shared<KeyHeldEvent>(held.at(i), duration)));
    }
}

void InputHandler::handleScrollInput(GLFWwindow* window, double offsetX, double offsetY, EventQueue* e, TimerMap* timerMap) {
    double sensitivity = 2;
    e->callEvent(std::shared_ptr<ScrollEvent>(std::make_shared<ScrollEvent>(sensitivity * offsetX, sensitivity * offsetY)));
}