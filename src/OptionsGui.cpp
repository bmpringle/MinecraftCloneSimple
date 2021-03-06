#include "OptionsGui.h"

OptionsGui::OptionsGui(Renderer* renderer, GameSettings* settings) : done(Button(0, 0, 0, 160, "Done", renderer)), renderer(renderer), settings(settings) {
    done.autoSize(0, -900);
    done.setLayer(-7);

    scrollY = -200;

    minScrollY = scrollY;

    int buttonOffset = 0;

    for(std::pair<Setting, std::string> setting : settings->getSettingsMap()) {
        Button b = Button(0, 0, 0, 200, GameSettings::settingToString(setting.first) + ": " + displaySetting(setting.second), renderer);

        b.autoSize(0, 800 + buttonOffset);

        optionButtons[setting.first] = b;
        
        buttonOffset -= 250;
    }
    maxScrollY = -1600 - buttonOffset;
}

void OptionsGui::displayGui(Renderer* renderer, int mouseX, int mouseY) {
    done.render(renderer);

    for(std::pair<Setting, Button> button : optionButtons) {
        button.second.render(renderer, 0, scrollY);
    }

    float cutout[36] {
        -1000, 800, -6, 0, 0, 0,
        1000, 800, -6, 0, 0, 0,
        -1000, 1000, -6, 0, 0, 0,
        -1000, 1000, -6, 0, 0, 0,
        1000, 800, -6, 0, 0, 0,
        1000, 1000, -6, 0, 0, 0,
    };

    renderer->renderRectangle(cutout);

    float cutout2[36] = {
        -1000, -1000, -6, 0, 0, 0,
        1000, -1000, -6, 0, 0, 0,
        -1000, -800, -6, 0, 0, 0,
        -1000, -800, -6, 0, 0, 0,
        1000, -1000, -6, 0, 0, 0,
        1000, -800, -6, 0, 0, 0,
    };

    renderer->renderRectangle(cutout2);
}

int OptionsGui::getID() {
    return 2;
}

void OptionsGui::mouseClick(int mouseX, int mouseY) {
    if(done.isBeingHoveredOver(mouseX, mouseY, renderer->getWidth(), renderer->getHeight())) {
        done.setPressed();
        return;
    }else {
        done.isPressed();
    }

    for(std::pair<Setting, Button> button : optionButtons) {
        if(button.second.isBeingHoveredOver(mouseX, mouseY, renderer->getWidth(), renderer->getHeight(), 0, scrollY)) {
            settingToSet = button.first;
            button.second.setPressed();
        }else {
            done.isPressed();
        }
    }
}

void OptionsGui::scrollHandle(double offsetX, double offsetY) {
    this->scrollY += offsetY;

    if(scrollY < minScrollY) {
        scrollY = minScrollY;
    }

    if(scrollY > maxScrollY) {
        scrollY = maxScrollY;
    }
}

void OptionsGui::close() {

}

void OptionsGui::handleOptionInput(std::string input) {
    if(settingToSet == DUMMY_SETTING) {
        return;
    }

    settings->setSetting(settingToSet, input);
    Button oldButton = optionButtons[settingToSet];
    double middleX = oldButton.getX() + (double)oldButton.getWidth() / 2.0;
    double middleY = oldButton.getY() + (double)oldButton.getHeight() / 2.0;

    optionButtons[settingToSet] = Button(0, 0, 0, 200, GameSettings::settingToString(settingToSet) + ": " + displaySetting(settings->getSetting(settingToSet)), renderer);
    
    optionButtons[settingToSet].autoSize(middleX, middleY);
    
    settingToSet = DUMMY_SETTING;
}