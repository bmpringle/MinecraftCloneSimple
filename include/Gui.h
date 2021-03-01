#ifndef GUI_H
#define GUI_H

#include "WorldRenderer.h"

class Gui {
    public:
        virtual void displayGui(WorldRenderer* renderer, int mouseX, int mouseY) = 0;
        virtual int getID() = 0;
        virtual int getIndexHoveredOver(WorldRenderer* renderer, int mouseX, int mouseY) = 0;
        virtual void mouseClick(int mouseX, int mouseY) = 0;
        virtual void close() = 0;
        virtual ~Gui() = default;
};
#endif