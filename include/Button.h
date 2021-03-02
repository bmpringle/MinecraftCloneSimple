#ifndef BUTTON_H
#define BUTTON_H

#include "RenderInclude.h"
#include <string>
#include "WorldRenderer.h"

class Button {
    public:
        Button(int x, int y, int width, int height, std::string texture, WorldRenderer* renderer);

        bool isBeingHoveredOver(int mouseX, int mouseY, int windowWidth, int windowHeight);

        void render(WorldRenderer* renderer);

        bool isPressed();

        void setPressed();

    private:
        float x;
        float y;
        float width;
        float height;
        unsigned int TBO = -1;

        bool pressed = false;
};
#endif