#ifndef BLOCKDOOR_H
#define BLOCKDOOR_H

#include "Block.h"

class BlockDoor : public Block {
    public:
        BlockDoor();
        
        std::string getName() override;

        std::string getTextureName(SideEnum side, int data) override;  

        int getXRotation(int data) override;

        int getYRotation(int data) override;

        int getZRotation(int data) override;

        void onPlaced(SideEnum hPlacementAngle, SideEnum sideLookingAt, int* data) override;

        BlockRenderedModel getRenderedModel(int data) override;

        AABB getAABB(int data) override;

        bool onBlockActivated(World* world, BlockPos pos, ItemStack* stack, int* data) override;

        void updateBlock(BlockArrayData* data, BlockData* blockToUpdate) override;
};
#endif