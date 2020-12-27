#include "Game.h"
#include <iostream>

GLFWwindow* setup() {

    if (!glfwInit()) {
        // Initialization failed
        glfwTerminate();
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(640, 480, "Minecraft Clone Simple", NULL, NULL);

    if(!window) {
        std::cout << "window == nullptr, something has gone very wrong." << std::endl;
        abort();
    }

    glfwMakeContextCurrent(window);


    glewExperimental = GL_TRUE;
    glewInit();

    return window;
}

int main() {

    Game game = Game(setup());

    game.start();
}