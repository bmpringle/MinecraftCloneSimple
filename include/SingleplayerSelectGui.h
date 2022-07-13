#ifndef SINGLEPLAYERSELECTGUI_H
#define SINGLEPLAYERSELECTGUI_H

#include "Gui.h"
#include "Button.h"

class SingleplayerSelectGui : public Gui {
    public:
        SingleplayerSelectGui(VulkanRenderer* renderer);

        void handleKeyPressed(std::string keyPressed);
        void displayGui(VulkanRenderer* renderer, int mouseX, int mouseY) override;
        int getID() override;
        void mouseClick(int mouseX, int mouseY) override;
        void scrollHandle(double offsetX, double offsetY) override;
        void close() override;
        void open() override;

        std::string worldName = "";
        int worldSeed = -1;
        bool done = false;

    private:
        Button createNewWorld;
        Button loadWorld;

        Button textBuffer;

        Button textPrefix;

        VulkanRenderer* renderer;

        //0 is select screen, 1 is create screen, 2 is load screen.
        int state = 0;
};
#endif