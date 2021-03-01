#include "InventoryGui.h"

InventoryGui::InventoryGui(WorldRenderer* renderer, Inventory* inv) : inventory(inv), renderer(renderer) {

}

void InventoryGui::displayGui(WorldRenderer* renderer, int mouseX, int mouseY) {
    
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

    renderer->renderOverlay(guioverlay, background);

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

        if(!stack.isEmpty()) {
            float itemoverlay[48] = {
                modX + rowStartX + xStart, modY + rowStartY, -4, 0, 0, 1, 0, 0,
                modX + rowStartX + end, modY + rowStartY, -4, 0, 0, 1, 1, 0,
                modX + rowStartX + xStart, modY + rowStartY + endY, -4, 0, 0, 1, 0, 1,

                modX + rowStartX + xStart, modY + rowStartY + endY, -4, 0, 0, 1, 0, 1,
                modX + rowStartX + end, modY + rowStartY, -4, 0, 0, 1, 1, 0,
                modX + rowStartX + end, modY + rowStartY + endY, -4, 0, 0, 1, 1, 1
            };

            renderer->renderOverlay(itemoverlay, stack.getItem()->getIcon());
        }   
        double mouseOverlayX = (double)mouseX / (double)renderer->getWidth() * dims[0] * 2 - dims[0];
        double mouseOverlayY = (dims[1] * (2 * (1 - (double)mouseY / (double)renderer->getHeight()) - 1)) * (double)renderer->getHeight() / (double)renderer->getWidth();

        if(mouseOverlayX >= modX + rowStartX + xStart && mouseOverlayX <= modX + rowStartX + end) {
            if(mouseOverlayY >= modY + rowStartY && mouseOverlayY <= modY + rowStartY + endY) {
                float selectoverlay[48] = {
                    modX + rowStartX + xStart, modY + rowStartY, -5, 0, 0, 1, 0, 0,
                    modX + rowStartX + end, modY + rowStartY, -5, 0, 0, 1, 1, 0,
                    modX + rowStartX + xStart, modY + rowStartY + endY, -5, 0, 0, 1, 0, 1,

                    modX + rowStartX + xStart, modY + rowStartY + endY, -5, 0, 0, 1, 0, 1,
                    modX + rowStartX + end, modY + rowStartY, -5, 0, 0, 1, 1, 0,
                    modX + rowStartX + end, modY + rowStartY + endY, -5, 0, 0, 1, 1, 1
                };
                renderer->renderOverlay(selectoverlay, "inventory_select.png");
            }
        }

        if(!itemStackHeld.isEmpty()) {
            float xSize = modX + rowStartX + end - (modX + rowStartX + xStart);
            float ySize = modY + rowStartY + endY - (modY + rowStartY);
            float xPos = mouseOverlayX - xSize / 2;
            float yPos = mouseOverlayY - ySize / 2;
            float selectoverlay[48] = {
                xPos, yPos, -5, 0, 0, 1, 0, 0,
                xPos + xSize, yPos, -5, 0, 0, 1, 1, 0,
                xPos, yPos + ySize, -5, 0, 0, 1, 0, 1,

                xPos, yPos + ySize, -5, 0, 0, 1, 0, 1,
                xPos + xSize, yPos, -5, 0, 0, 1, 1, 0,
                xPos + xSize, yPos + ySize, -5, 0, 0, 1, 1, 1
            };
            renderer->renderOverlay(selectoverlay, itemStackHeld.getItem()->getIcon());
        }
    }
}

int InventoryGui::getID() {
    return 1;
}

int InventoryGui::getIndexHoveredOver(WorldRenderer* renderer, int mouseX, int mouseY) {
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
        ItemStack stack = inventory->getItemStackInSlot(hoveringOver);
        if(stack.isEmpty()) {
            if(!itemStackHeld.isEmpty()) {
                inventory->setItemStackInSlot(hoveringOver, itemStackHeld);
                itemStackHeld = ItemStack(nullptr, 0);
            }
        }else {
            if(itemStackHeld.isEmpty()) {
                itemStackHeld = stack;
                inventory->setItemStackInSlot(hoveringOver, ItemStack(nullptr, 0));
            }
        }
    }else {
        //todo: drop item.
    }
}

void InventoryGui::close() {
    
}