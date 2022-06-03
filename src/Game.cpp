#include "Game.h"
#include "MainMenuGui.h"
#include "OptionsGui.h"
#include "SingleplayerSelectGui.h"
#include "PlatformFilesystem.h"

Game::Game(GLFWwindow* _window) : eventQueue(EventQueue()), map(TimerMap()), input(InputHandler()), renderer(Renderer()), world(nullptr), gameEventHandler(std::make_shared<GameEventHandler>(GameEventHandler(this))), gui(std::make_shared<MainMenuGui>(&renderer)), settings(GameSettings()) {
    settings.initDefaultSettings();
    settings.parseOptionsFromFile("src/assets/options.txt");
    
    eventQueue.addEventListener(gameEventHandler);

    glfwSetWindowUserPointer(renderer.getWindowPtr(), this);

    auto func = [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        static_cast<Game*>(glfwGetWindowUserPointer(w))->internalKeyCallback(w, key, scancode, action, mods);
    };

    auto func2 = [](GLFWwindow* w, double xpos, double ypos) {
        static_cast<Game*>(glfwGetWindowUserPointer(w))->internalMouseCallback(w, xpos, ypos);
    };

    auto func3 = [](GLFWwindow* w, int button, int action, int mods) {
        static_cast<Game*>(glfwGetWindowUserPointer(w))->internalMouseButtonCallback(w, button, action, mods);
    };

    auto func4 = [](GLFWwindow* w, double offsetX, double offsetY) {
        static_cast<Game*>(glfwGetWindowUserPointer(w))->internalScrollCall(w, offsetX, offsetY);
    };

    glfwSetKeyCallback(renderer.getWindowPtr(), func);
    glfwSetCursorPosCallback(renderer.getWindowPtr(), func2);
    glfwSetMouseButtonCallback(renderer.getWindowPtr(), func3);
    glfwSetScrollCallback(renderer.getWindowPtr(), func4);
}

void Game::start() {    
    while(!glfwWindowShouldClose(renderer.getWindowPtr())) {
        renderer.updateAspectRatio(renderer.getWindowPtr());
        
        gui->displayGui(&renderer, 0, 0);
        renderer.renderFrame(nullptr);

        if(gui->getID() == 1) {
            std::shared_ptr<MainMenuGui> menuGui = std::dynamic_pointer_cast<MainMenuGui>(gui);
            if(menuGui->singleplayer.isPressed()) {
                if(world != nullptr) {
                    world = nullptr;
                }
                gui->close();
                gui = std::make_shared<SingleplayerSelectGui>(&renderer);
            }
            
            if(menuGui->quit.isPressed()){
                break;
            }

            if(menuGui->options.isPressed()) {
                gui->close();
                gui = std::make_shared<OptionsGui>(&renderer, &settings);
            }
        }else if(gui->getID() == 2) {
            std::shared_ptr<OptionsGui> optionsGui = std::dynamic_pointer_cast<OptionsGui>(gui);
            if(optionsGui->done.isPressed()) {
                gui->close();
                gui = std::make_shared<MainMenuGui>(&renderer);
            }
        }else if(gui->getID() == 3) {
            std::shared_ptr<SingleplayerSelectGui> singleplayerGui = std::dynamic_pointer_cast<SingleplayerSelectGui>(gui);
            if(singleplayerGui->done) {
                if(world != nullptr) {
                    world = nullptr;
                }

                std::string worldname = singleplayerGui->worldName;
                int seed = singleplayerGui->worldSeed;
                try {
                    if(seed == -1) {
                        std::ifstream seedIn("./worlds/"+worldname+"/data/seed.cdat");
                        std::string buffer;
                        std::getline(seedIn, buffer);
                        world = std::make_shared<World>(renderer.getWindowPtr(), &eventQueue, &input, &renderer, &map, &settings, worldname, "./worlds/"+worldname+"/", std::stoi(buffer));
                    }
                }catch(std::exception e) {
                    std::cout << "failed to load world. quitting to main menu" << std::endl;
                    gui->close();
                    gui = std::make_shared<MainMenuGui>(&renderer);
                }
                if(seed != -1) {
                    if(fs::exists("./worlds/"+worldname+"/data/")) {
                        std::cout << "world already exists. will not overwrite. quitting to main menu" << std::endl;
                        gui->close();
                        gui = std::make_shared<MainMenuGui>(&renderer);
                    }else {
                        fs::create_directories("./worlds/"+worldname+"/data/");
                        world = std::make_shared<World>(renderer.getWindowPtr(), &eventQueue, &input, &renderer, &map, &settings, worldname, "./worlds/"+worldname+"/", seed);
                    }
                }

                if(world != nullptr) {
                    gui->close();
                    world->resume();
                }
            }
        }

        input.callRegularEvents(&eventQueue, &map);
        #ifndef VULKAN_BACKEND
        glfwSwapBuffers(renderer.getWindowPtr());
        #endif
        glfwPollEvents();
    }
    #ifndef VULKAN_BACKEND
    glfwDestroyWindow(renderer.getWindowPtr());
    glfwTerminate();
    #endif

    settings.saveOptionsToFile("src/assets/options.txt");
}

void Game::pauseOrResume() {
    if(world != nullptr) {
        if(world->isPaused()) {
            world->resume();
        }else {
            world->pause();
        } 
    }
}

std::shared_ptr<Gui> Game::getGui() {
    return gui;
}

void Game::internalKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    input.handleInput(window, key, scancode, action, mods, &eventQueue, &map);
}

void Game::internalMouseCallback(GLFWwindow* window, double xpos, double ypos) {
    input.handleMouseInput(window, xpos, ypos, &eventQueue, &map);
}

void Game::internalMouseButtonCallback(GLFWwindow* w, int button, int action, int mods) {
    input.handleMouseButtonInput(w, button, action, mods, &eventQueue, &map);
}

void Game::quitToMainMenu() {
    if(world != nullptr) {
        world->quit();
    }

    if(gui->getID() != 1) {
        gui->close();
        gui = std::make_shared<MainMenuGui>(&renderer);
    }
}

void Game::internalScrollCall(GLFWwindow* window, double offsetX, double offsetY) {
    input.handleScrollInput(window, offsetX, offsetY, &eventQueue, &map);
}