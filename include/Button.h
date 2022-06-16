#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include "VulkanRenderer.h"

class Button {
    public:
        Button(int x, int y, int width, int height, std::string texture, VulkanRenderer* renderer);

        Button();

        bool isBeingHoveredOver(int mouseX, int mouseY, int windowWidth, int windowHeight);

        bool isBeingHoveredOver(int mouseX, int mouseY, int windowWidth, int windowHeight, double offsetX, double offsetY);

        void setRenderData(VulkanRenderer* renderer);

        void setRenderData(VulkanRenderer* renderer, double offsetX, double offsetY);

        void stopRendering(VulkanRenderer* renderer);

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

        void setText(VulkanRenderer* renderer, std::string text);

        std::string getTextureID();

        std::string getText();

        void setRenderButtonBackground(bool background);

    private:
        float x;
        float y;
        float width;
        float height;

        float layer = -5;

        std::string id = "-1";

        static unsigned int buttonCount;

        int b_w = 0;
        int b_h = 0;

        bool pressed = false;

        std::string text;

        std::string textureID;

        std::string renderID;

        bool renderButtonBackground = false;
};
#endif