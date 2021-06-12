#include "Button.h"

Button::Button(int x, int y, int width, int height, std::string texture, Renderer* renderer) : x(x), y(y), width(width), height(height), text(texture) {
    setText(renderer, text);
}

Button::Button() : x(0), y(0), width(0), height(0), text("") {

}

bool Button::isBeingHoveredOver(int mouseX, int mouseY, int windowWidth, int windowHeight) {
    return isBeingHoveredOver(mouseX, mouseY, windowWidth, windowHeight, 0, 0);
}

bool Button::isBeingHoveredOver(int mouseX, int mouseY, int windowWidth, int windowHeight, double offsetX, double offsetY) {
    float aspect = (double)windowWidth / (double)windowHeight;

    float xCheck = (float)mouseX / (float)windowWidth * 2000 - 1000;
    float yCheck = -((float)mouseY / (float)windowHeight * 2000 - 1000) * windowHeight / windowWidth;

    if(xCheck >= (x + offsetX) && xCheck <= x + width + offsetX) {
        if(yCheck * aspect >= y + offsetY && yCheck * aspect <= y + height + offsetY) {
            return true;
        }
    }
    return false;
}

void Button::render(Renderer* renderer, double offsetX, double offsetY) {
    float aspect = (double)renderer->getHeight() / (double)renderer->getWidth();

    float numberoverlay[48] = {
        (float)(x + offsetX) * aspect, (float)(y + offsetY) * aspect, layer, 0, 0, 1, 0, 0,
        (float)(x + width + offsetX) * aspect, (float)(y + offsetY) * aspect, layer, 0, 0, 1, 1, 0,
        (float)(x + offsetX) * aspect, (float)(y + height + offsetY) * aspect, layer, 0, 0, 1, 0, 1,

        (float)(x + offsetX) * aspect, (float)(y + height + offsetY) * aspect, layer, 0, 0, 1, 0, 1,
        (float)(x + width + offsetX) * aspect, (float)(y + offsetY) * aspect, layer, 0, 0, 1, 1, 0,
        (float)(x + width + offsetX) * aspect, (float)(y + height + offsetY) * aspect, layer, 0, 0, 1, 1, 1
    };

    renderer->renderOverlay(numberoverlay, TBO);
}

void Button::render(Renderer* renderer) {
    render(renderer, 0, 0);
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

    double conversionFactor = (double)b_w / (double)b_h;

    int _width = height * conversionFactor;
    int _x = middleX - _width / 2.0;

    x = _x;
    y = _y;
    width = _width;
    height = _height;
}

double Button::getX() {
    return x;
}

double Button::getY() {
    return y;
}

double Button::getWidth() {
    return width;
}

double Button::getHeight() {
    return height;
}

void Button::setLayer(double layer) {
    this->layer = layer;
}

void Button::setText(Renderer* renderer, std::string text) {
    this->text = text;
    glDeleteTextures(1, &TBO);
    
    if(text == "") {
        TBOinit = true;
        TBO = renderer->textTextureBuffer(" ");
        b_h = renderer->getBitmapHeight();
        b_w = renderer->getBitmapWidth();
        return;
    }
    TBOinit = true;
    TBO = renderer->textTextureBuffer(text);
    b_h = renderer->getBitmapHeight();
    b_w = renderer->getBitmapWidth();
    autoSize(x + width/2.0, y + height/2.0);
}

std::string Button::getText() {
    return text;
}

void Button::setX(double x) {
    this->x = x;
}

void Button::setY(double y) {
    this->y = y;
}

void Button::setWidth(double width) {
    this->width = width;
}

void Button::setHeight(double height) {
    this->height = height;
}