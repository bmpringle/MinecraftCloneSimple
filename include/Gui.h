#ifndef GUI_H
#define GUI_H

#include "Renderer.h"

class Gui {
    public:
        virtual void displayGui(Renderer* renderer, int mouseX, int mouseY) = 0;
        virtual int getID() = 0;

        virtual void mouseClick(int mouseX, int mouseY) = 0;
        virtual void scrollHandle(double offsetX, double offsetY) = 0;
        virtual void close() = 0;
        virtual ~Gui() = default;
};
#endif