#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include "Renderer.h"

class Button {
    public:
        Button(int x, int y, int width, int height, std::string texture, Renderer* renderer);

        Button();

        bool isBeingHoveredOver(int mouseX, int mouseY, int windowWidth, int windowHeight);

        bool isBeingHoveredOver(int mouseX, int mouseY, int windowWidth, int windowHeight, double offsetX, double offsetY);

        void render(Renderer* renderer);

        void render(Renderer* renderer, double offsetX, double offsetY);

        bool isPressed();

        void setPressed();

        void autoSize(int middleX, int middleY);

        double getX();

        double getY();

        double getWidth();

        double getHeight();

        void setX(double x);

        void setY(double y);

        void setWidth(double width);

        void setHeight(double height);

        void setLayer(double layer);

        void setText(Renderer* renderer, std::string text);

        std::string getText();

    private:
        float x;
        float y;
        float width;
        float height;

        float layer = -5;

        unsigned int TBO = 0;
        bool TBOinit = false;

        int b_w = 0;
        int b_h = 0;

        bool pressed = false;

        std::string text;
};
#endif