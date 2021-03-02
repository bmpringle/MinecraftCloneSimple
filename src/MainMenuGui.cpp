#include "MainMenuGui.h"

MainMenuGui::MainMenuGui(WorldRenderer* renderer) : singleplayer(Button(-400, 200, 800, 200, "Singleplayer", renderer)), quit(Button(-400, -400, 800, 200, "Quit Game", renderer)), options(Button(-400, -100, 800, 200, "Options (No-OP)", renderer)), renderer(renderer) {

}

void MainMenuGui::displayGui(WorldRenderer* renderer, int mouseX, int mouseY) {
    singleplayer.render(renderer);
    quit.render(renderer);
    options.render(renderer);
}

int MainMenuGui::getID() {
    return 1;
}

void MainMenuGui::mouseClick(int mouseX, int mouseY) {
    if(singleplayer.isBeingHoveredOver(mouseX, mouseY, renderer->getWidth(), renderer->getHeight())) {
        singleplayer.setPressed();
    }else {
        singleplayer.isPressed();
    }

    if(quit.isBeingHoveredOver(mouseX, mouseY, renderer->getWidth(), renderer->getHeight())) {
        quit.setPressed();
    }else {
        quit.isPressed();
    }

    if(options.isBeingHoveredOver(mouseX, mouseY, renderer->getWidth(), renderer->getHeight())) {
        options.setPressed();
    }else {
        options.isPressed();
    }
}

void MainMenuGui::close() {

}