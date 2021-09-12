#include "InventoryGui.h"

InventoryGui::InventoryGui(Renderer* renderer, Inventory* inv) : inventory(inv), renderer(renderer) {

}

void InventoryGui::displayGui(Renderer* renderer, int mouseX, int mouseY) {
    
    std::array<int, 2> dims = renderer->overlayDimensions();
    float modX = dims[0] * x * 0.8;
    float modY = dims[1] * y * 0.75;
    float modXSize = dims[0] * xSize * 1.2;
    float modYSize = dims[1] * ySize;

    float guioverlay[48] = {
        modX, modY, -3, 0, 0, 1, 0, 0,
        modX + modXSize, modY, -3, 0, 0, 1, 1, 0,
        modX, modY + modYSize, -3, 0, 0, 1, 0, 1,

        modX, modY + modYSize, -3, 0, 0, 1, 0, 1,
        modX + modXSize, modY, -3, 0, 0, 1, 1, 0,
        modX + modXSize, modY + modYSize, -3, 0, 0, 1, 1, 1
    };

    renderer->setOverlayData("inventory-background", guioverlay, background);

    for(int i = 0; i < 36; ++i) {
        ItemStack stack = inventory->getItemStackInSlot(i);
        float rowStartX = texStartX / backgroundTexX * dims[0];
        float rowStartY = 0;

        if(i < 9) {
            rowStartY = hotbarTexY / backgroundTexY * dims[1];
        }else if(i >= 9 && i < 18) {
            rowStartY = row1TexY / backgroundTexY * dims[1];
        }else if(i >= 18 && i < 27) {
            rowStartY = row2TexY / backgroundTexY * dims[1];
        }else {
            rowStartY = row3TexY / backgroundTexY * dims[1];
        }

        float xStart = 3.85 * (i % 9) / backgroundTexX * dims[0] + (i % 9) * itemSizeX / backgroundTexX * dims[0];
        float end = 3.85 * (i % 9) / backgroundTexX * dims[0] + ((i % 9) + 1) * itemSizeX / backgroundTexX * dims[0];
        float endY = itemSizeY / backgroundTexY * dims[1];


        double mouseOverlayX = (double)mouseX / (double)renderer->getWidth() * dims[0] * 2 - dims[0];
        double mouseOverlayY = (dims[1] * (2 * (1 - (double)mouseY / (double)renderer->getHeight()) - 1)) * (double)renderer->getHeight() / (double)renderer->getWidth();

        if(mouseOverlayX >= modX + rowStartX + xStart && mouseOverlayX <= modX + rowStartX + end) {
            if(mouseOverlayY >= modY + rowStartY && mouseOverlayY <= modY + rowStartY + endY) {
                float selectoverlay[48] = {
                    modX + rowStartX + xStart, modY + rowStartY, -8, 0, 0, 1, 0, 0,
                    modX + rowStartX + end, modY + rowStartY, -8, 0, 0, 1, 1, 0,
                    modX + rowStartX + xStart, modY + rowStartY + endY, -8, 0, 0, 1, 0, 1,

                    modX + rowStartX + xStart, modY + rowStartY + endY, -8, 0, 0, 1, 0, 1,
                    modX + rowStartX + end, modY + rowStartY, -8, 0, 0, 1, 1, 0,
                    modX + rowStartX + end, modY + rowStartY + endY, -8, 0, 0, 1, 1, 1
                };
                renderer->setOverlayData("inventory-select-overlay-" + std::to_string(i), selectoverlay, "inventory_select.png");
            }else {
                renderer->removeOverlayData("inventory-select-overlay-" + std::to_string(i));
            }
        }else {
            renderer->removeOverlayData("inventory-select-overlay-" + std::to_string(i));
        }

        if(!stack.isEmpty()) {
            float itemoverlay[48] = {
                modX + rowStartX + xStart, modY + rowStartY, -4, 0, 0, 1, 0, 0,
                modX + rowStartX + end, modY + rowStartY, -4, 0, 0, 1, 1, 0,
                modX + rowStartX + xStart, modY + rowStartY + endY, -4, 0, 0, 1, 0, 1,

                modX + rowStartX + xStart, modY + rowStartY + endY, -4, 0, 0, 1, 0, 1,
                modX + rowStartX + end, modY + rowStartY, -4, 0, 0, 1, 1, 0,
                modX + rowStartX + end, modY + rowStartY + endY, -4, 0, 0, 1, 1, 1
            };

            renderer->setOverlayData("inventory-stack-" + std::to_string(i), itemoverlay, stack.getItem()->getIcon());

            float xSize = (end - xStart) * 0.35;
            float ySize = (endY) * 0.40;
            float xPos = modX + rowStartX + xStart + xSize * 1.8;
            float yPos = modY + rowStartY + ySize * 0.2;

            float numberoverlay[48] = {
                xPos, yPos, -5, 0, 0, 1, 0, 0,
                xPos + xSize, yPos, -5, 0, 0, 1, 1, 0,
                xPos, yPos + ySize, -5, 0, 0, 1, 0, 1,

                xPos, yPos + ySize, -5, 0, 0, 1, 0, 1,
                xPos + xSize, yPos, -5, 0, 0, 1, 1, 0,
                xPos + xSize, yPos + ySize, -5, 0, 0, 1, 1, 1
            };

            renderer->setOverlayData("inventory-stack-count-" + std::to_string(i), numberoverlay, stack.getCountTextureID(renderer));
        }else {
            renderer->removeOverlayData("inventory-stack-" + std::to_string(i));
            renderer->removeOverlayData("inventory-stack-count-" + std::to_string(i));
        }   

        if(!itemStackHeld.isEmpty()) {
            float xSize = (end - xStart);
            float ySize = endY;
            float xPos = mouseOverlayX - xSize / 2;
            float yPos = (mouseOverlayY - ySize / 2);

            float selectoverlay[48] = {
                xPos, yPos, -9, 0, 0, 1, 0, 0,
                xPos + xSize, yPos, -9, 0, 0, 1, 1, 0,
                xPos, yPos + ySize, -9, 0, 0, 1, 0, 1,

                xPos, yPos + ySize, -9, 0, 0, 1, 0, 1,
                xPos + xSize, yPos, -9, 0, 0, 1, 1, 0,
                xPos + xSize, yPos + ySize, -9, 0, 0, 1, 1, 1
            };
            renderer->setOverlayData("inventory-item-stack-held", selectoverlay, itemStackHeld.getItem()->getIcon());

            xSize = (end - xStart) * 0.35;
            ySize = (endY) * 0.40;
            xPos = mouseOverlayX + xSize * 0.40;
            yPos = mouseOverlayY - ySize;

            float numberoverlay[48] = {
                xPos, yPos, -10, 0, 0, 1, 0, 0,
                xPos + xSize, yPos, -10, 0, 0, 1, 1, 0,
                xPos, yPos + ySize, -10, 0, 0, 1, 0, 1,

                xPos, yPos + ySize, -10, 0, 0, 1, 0, 1,
                xPos + xSize, yPos, -10, 0, 0, 1, 1, 0,
                xPos + xSize, yPos + ySize, -10, 0, 0, 1, 1, 1
            };
            renderer->setOverlayData("inventory-item-stack-held-count", numberoverlay, itemStackHeld.getCountTextureID(renderer));
        }else {
            renderer->removeOverlayData("inventory-item-stack-held");
            renderer->removeOverlayData("inventory-item-stack-held-count");
        }
    }
}

int InventoryGui::getID() {
    return 0;
}

int InventoryGui::getIndexHoveredOver(Renderer* renderer, int mouseX, int mouseY) {
    std::array<int, 2> dims = renderer->overlayDimensions();
    float modX = dims[0] * x * 0.8;
    float modY = dims[1] * y * 0.75;

    for(int i = 0; i < 36; ++i) {
        ItemStack stack = inventory->getItemStackInSlot(i);
        float rowStartX = texStartX / backgroundTexX * dims[0];
        float rowStartY = 0;

        if(i < 9) {
            rowStartY = hotbarTexY / backgroundTexY * dims[1];
        }else if(i >= 9 && i < 18) {
            rowStartY = row1TexY / backgroundTexY * dims[1];
        }else if(i >= 18 && i < 27) {
            rowStartY = row2TexY / backgroundTexY * dims[1];
        }else {
            rowStartY = row3TexY / backgroundTexY * dims[1];
        }

        float xStart = 3.85 * (i % 9) / backgroundTexX * dims[0] + (i % 9) * itemSizeX / backgroundTexX * dims[0];
        float end = 3.85 * (i % 9) / backgroundTexX * dims[0] + ((i % 9) + 1) * itemSizeX / backgroundTexX * dims[0];
        float endY = itemSizeY / backgroundTexY * dims[1];

        double mouseOverlayX = (double)mouseX / (double)renderer->getWidth() * dims[0] * 2 - dims[0];
        double mouseOverlayY = (dims[1] * (2 * (1 - (double)mouseY / (double)renderer->getHeight()) - 1)) * (double)renderer->getHeight() / (double)renderer->getWidth();

        if(mouseOverlayX >= modX + rowStartX + xStart && mouseOverlayX <= modX + rowStartX + end) {
            if(mouseOverlayY >= modY + rowStartY && mouseOverlayY <= modY + rowStartY + endY) {
                return i;
            }
        }
    }
    return -1;
}

void InventoryGui::mouseClick(int mouseX, int mouseY) {
    int hoveringOver = getIndexHoveredOver(renderer, mouseX, mouseY);
    if(hoveringOver != -1) {
        itemStackHeld = inventory->interactHeldItemAtSlot(itemStackHeld, hoveringOver);
    }else {
        //todo: drop item.
    }
}

void InventoryGui::close() {
    renderer->removeOverlayData("inventory-background");

    for(int i = 0; i < 36; ++i) {
        renderer->removeOverlayData("inventory-stack-" + std::to_string(i));
        renderer->removeOverlayData("inventory-stack-count-" + std::to_string(i));
        renderer->removeOverlayData("inventory-select-overlay-" + std::to_string(i));
    }

    renderer->removeOverlayData("inventory-item-stack-held");
    renderer->removeOverlayData("inventory-item-stack-held-count");

    renderer->removeOverlayData("inventory-item-stack-held-count");
}

void InventoryGui::scrollHandle(double offsetX, double offsetY) { 
    
}