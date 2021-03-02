#include "Game.h"
#include <iostream>
#include "FontLoader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
} 

GLFWwindow* setup(int x, int y) {

    if (!glfwInit()) {
        // Initialization failed
        glfwTerminate();
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    
    GLFWwindow* window = glfwCreateWindow(x, y, "Minecraft Clone Simple", NULL, NULL);

    if(!window) {
        std::cout << "window == nullptr, something has gone very wrong." << std::endl;
        abort();
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE;
    glewInit();

    printf("Opengl Version being used is: %s\n", glGetString(GL_VERSION));

    return window;
}

int main(int argc, char** argv) {
    srand(time(NULL));
    
    int x = 640;
    int y = 480;

    if(argc == 3) {
        x = std::stoi(argv[1]);
        y = std::stoi(argv[2]);
    }
    Game game = Game(setup(x, y));

    game.start();
}