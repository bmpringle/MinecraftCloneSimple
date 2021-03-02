#ifndef MAINMENUGUI_H
#define MAINMENUGUI_H
#include "Gui.h"
#include "Button.h"

class MainMenuGui : public Gui {
    public:
        MainMenuGui(WorldRenderer* renderer);
        void displayGui(WorldRenderer* renderer, int mouseX, int mouseY);
        int getID();
        void mouseClick(int mouseX, int mouseY);
        void close();

        Button singleplayer;
        Button quit;
        Button options;

    private:
        WorldRenderer* renderer;
};
#endif