#ifndef BLOCKDATA_H
#define BLOCKDATA_H

#include "Block.h"
#include "BlockPos.h"

class ItemStack;
class World;
class BlockArrayData;

class BlockData {
    public:
        BlockData(std::shared_ptr<Block> type, BlockPos pos);

        BlockData();

        BlockPos getPos();

        void setPos(BlockPos pos_);

        AABB getAABB();

        std::shared_ptr<Block> getBlockType();

        int getData();

        void setData(int data);

        BlockRenderedModel getRenderedModel();

        std::string getTextureName(SideEnum side);

        bool isSolid();

        bool isOpaque();

        void placedOnSide(SideEnum hPlacementAngle, SideEnum sideLookingAt);

        void updateBlock(BlockArrayData* data);

        bool activateBlock(World* world, ItemStack* stack);

    private:
        BlockPos pos;
        std::shared_ptr<Block> type;
        int data = 0;
};
#endif