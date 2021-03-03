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

        void autoSize(int middleX, int middleY);

    private:
        float x;
        float y;
        float width;
        float height;

        unsigned int TBO = -1;
        int b_w = 0;
        int b_h = 0;

        bool pressed = false;
};
#endif