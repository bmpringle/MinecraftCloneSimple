#ifndef INVENTORYGUI_H
#define INVENTORYGUI_H
#include "Gui.h"
#include "Inventory.h"
#include "ItemStack.h"

class InventoryGui : public Gui  {
    public:
        InventoryGui(VulkanRenderer* renderer, Inventory* inv);
        void displayGui(VulkanRenderer* renderer, int mouseX, int mouseY) override;
        int getID() override;
        int getIndexHoveredOver(VulkanRenderer* renderer, int mouseX, int mouseY);
        void mouseClick(int mouseX, int mouseY) override;
        void scrollHandle(double offsetX, double offsetY) override;
        void close() override;
        void open() override;
    private:
        void renderItems();

        //bool itemsChanged = false;

        const std::string background = "inventory.png";
        const double x = -0.75;
        const double xSize = 1;
        const double y = -0.75;
        const double ySize = 1;
        ItemStack itemStackHeld = ItemStack(nullptr, 0);
        Inventory* inventory;
        VulkanRenderer* renderer;

        const double hotbarTexY = 15;
        const double row1TexY = 36;
        const double row2TexY = 55;
        const double row3TexY = 74;

        const double backgroundTexX = 200;
        const double backgroundTexY = 150;
        const double texStartX = 19;

        const double itemSizeX = 19;
        const double itemSizeY = 16;
};
#endif