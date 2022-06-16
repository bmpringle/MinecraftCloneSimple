#ifndef MAINMENUGUI_H
#define MAINMENUGUI_H
#include "Gui.h"
#include "Button.h"

class MainMenuGui : public Gui {
    public:
        MainMenuGui(VulkanRenderer* renderer);
        void displayGui(VulkanRenderer* renderer, int mouseX, int mouseY) override; 
        int getID() override;
        void mouseClick(int mouseX, int mouseY) override;
        void scrollHandle(double offsetX, double offsetY) override;
        void close() override;

        Button singleplayer;
        Button quit;
        Button options;

    private:
        VulkanRenderer* renderer;
};
#endif