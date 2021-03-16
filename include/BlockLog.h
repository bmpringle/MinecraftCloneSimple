#ifndef BLOCKLOG_H
#define BLOCKLOG_H

#include "Block.h"

class BlockLog : public Block {
    public:
        BlockLog();
        
        std::string getName() override;

        std::string getTextureName(SideEnum side, int data) override;  

        int getXRotation(int data) override;

        int getYRotation(int data) override;

        int getZRotation(int data) override;

        void onPlaced(SideEnum hPlacementAngle, SideEnum sideLookingAt, int* data) override;
};
#endif