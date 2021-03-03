#include "Button.h"

Button::Button(int x, int y, int width, int height, std::string texture, WorldRenderer* renderer) : x(x), y(y), width(width), height(height) {
    TBO = renderer->textTextureBuffer(texture);
    b_h = renderer->getBitmapHeight();
    b_w = renderer->getBitmapWidth();
}

bool Button::isBeingHoveredOver(int mouseX, int mouseY, int windowWidth, int windowHeight) {
    float xCheck = (float)mouseX / (float)windowWidth * 2000 - 1000;
    float yCheck = -((float)mouseY / (float)windowHeight * 2000 - 1000) * windowHeight / windowWidth;

    if(xCheck >= x && xCheck <= x + width) {
        if(yCheck >= y && yCheck <= y + height) {
            return true;
        }
    }
    return false;
}

void Button::render(WorldRenderer* renderer) {
    float numberoverlay[48] = {
        x, y, -5, 0, 0, 1, 0, 0,
        x + width, y, -5, 0, 0, 1, 1, 0,
        x, y + height, -5, 0, 0, 1, 0, 1,

        x, y + height, -5, 0, 0, 1, 0, 1,
        x + width, y, -5, 0, 0, 1, 1, 0,
        x + width, y + height, -5, 0, 0, 1, 1, 1
    };
    renderer->renderOverlay(numberoverlay, TBO);
}

void Button::setPressed() {
    pressed = true;
}

bool Button::isPressed() {
    bool temp = pressed;
    pressed = false;
    return temp;
}

void Button::autoSize(int middleX, int middleY) {
    int _height = height;
    int _y = middleY - _height / 2.0;

    double conversionFactor = b_w / b_h;

    int _width = height * conversionFactor;
    int _x = middleX - _width / 2.0;

    x = _x;
    y = _y;
    width = _width;
    height = _height;
}