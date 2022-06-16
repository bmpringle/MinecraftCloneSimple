#include "MainMenuGui.h"

MainMenuGui::MainMenuGui(VulkanRenderer* renderer) : singleplayer(Button(0, 0, 0, 200, "Singleplayer", renderer)), quit(Button(0, 0, 0, 200, "Quit Game", renderer)), options(Button(0, 0, 0, 200, "Options", renderer)), renderer(renderer) {
    singleplayer.autoSize(0, 400);
    quit.autoSize(0, -400);
    options.autoSize(0, 0);
}

void MainMenuGui::displayGui(VulkanRenderer* renderer, int mouseX, int mouseY) {
    singleplayer.setRenderData(renderer);
    quit.setRenderData(renderer);
    options.setRenderData(renderer);
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
    singleplayer.stopRendering(renderer);
    quit.stopRendering(renderer);
    options.stopRendering(renderer);
}

void MainMenuGui::scrollHandle(double offsetX, double offsetY) {

}