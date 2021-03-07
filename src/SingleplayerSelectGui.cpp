#include "SingleplayerSelectGui.h"

SingleplayerSelectGui::SingleplayerSelectGui(Renderer* renderer) : createNewWorld(Button(0, 0, 0, 200, "Create New World", renderer)), loadWorld(Button(0, 0, 0, 200, "Load World", renderer)), textBuffer(Button(0, 0, 0, 200, "", renderer)), textPrefix(Button(0, 0, 0, 200, "", renderer)), renderer(renderer) {
    createNewWorld.autoSize(0, 300);
    loadWorld.autoSize(0, -300);
    textBuffer.autoSize(0, 0);
    textPrefix.autoSize(0, 300);
}

void SingleplayerSelectGui::handleKeyPressed(std::string keyPressed) {
    if(keyPressed.length() == 1 && (state == 1 || state == 2)) {
        textBuffer.setText(renderer, textBuffer.getText() + keyPressed);
    }
    if(state == 1) {
        if(keyPressed == "ENTER") {
            if(worldName == "") {
                worldName = textBuffer.getText();
                textBuffer.setText(renderer, "");
                textPrefix.setText(renderer, "Enter new world seed:");
            }else {
                try {
                    int pSeed = std::stoi(textBuffer.getText());
                    if(!(pSeed == abs(pSeed))) {
                        throw std::exception();
                    }
                    worldSeed = pSeed;
                    textBuffer.setText(renderer, "");
                    done = true;
                }catch(std::exception e) {
                    std::cout << "could not convert seed to positive integer. please enter positive integer" << std::endl;
                }
            }
        }
    }else if(state == 2) {
        if(keyPressed == "ENTER") {
            if(worldName == "") {
                worldName = textBuffer.getText();
                textBuffer.setText(renderer, "");
                done = true;
            }
        }
    }

    if((state == 1 || state == 2) && keyPressed == "BACKSPACE") {
        std::string str = textBuffer.getText();
        if(str.length() >= 1) {
            str.erase(str.length() - 1, 1);
            textBuffer.setText(renderer, str);
        }
    }
}

void SingleplayerSelectGui::displayGui(Renderer* renderer, int mouseX, int mouseY) {
    if(state == 0) {
        createNewWorld.render(renderer);
        loadWorld.render(renderer);
    }else {
        textBuffer.render(renderer);
        textPrefix.render(renderer);
    }
}

int SingleplayerSelectGui::getID() {
    return 3;
}

void SingleplayerSelectGui::mouseClick(int mouseX, int mouseY) {
    if(state == 0) {
        if(createNewWorld.isBeingHoveredOver(mouseX, mouseY, renderer->getWidth(), renderer->getHeight())) {
            state = 1;
            textPrefix.setText(renderer, "Enter new world name:");
        }

        if(loadWorld.isBeingHoveredOver(mouseX, mouseY, renderer->getWidth(), renderer->getHeight())) {
            state = 2;
            textPrefix.setText(renderer, "Enter world to load:");
        }
    }
}

void SingleplayerSelectGui::scrollHandle(double offsetX, double offsetY) {
    
}

void SingleplayerSelectGui::close() {
    
}