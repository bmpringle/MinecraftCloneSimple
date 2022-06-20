#ifndef OPTIONSGUI_H
#define OPTIONSGUI_H

#include "Gui.h"
#include "Button.h"
#include "GameSettings.h"

class OptionsGui : public Gui {
    public:
        OptionsGui(VulkanRenderer* renderer, GameSettings* settings);
        void displayGui(VulkanRenderer* renderer, int mouseX, int mouseY) override;
        int getID() override;
        void mouseClick(int mouseX, int mouseY) override;
        void scrollHandle(double offsetX, double offsetY) override;
        void close() override;
        void handleOptionInput(std::string input);

        Button done;
        std::map<Setting, Button> optionButtons = std::map<Setting, Button>();

    private:
        VulkanRenderer* renderer;
        GameSettings* settings;

        Setting settingToSet = DUMMY_SETTING;

        int scrollY = 0;

        int minScrollY = 0;
        int maxScrollY = 0;

        std::function<std::string(std::string)> displaySetting = [](std::string s) {
            if(s == " ") {
                return std::string("SPACE");
            }
            return s;
        };

        std::string stringBuffer = "";
};
#endif