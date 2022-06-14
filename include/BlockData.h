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

        const std::shared_ptr<Block>& getBlockType();

        int getData();

        void setData(int data);

        BlockRenderedModel getRenderedModel();

        std::string getTextureName(SideEnum side);

        bool isSolid();

        bool isOpaque();

        bool isLiquid();

        void placedOnSide(SideEnum hPlacementAngle, SideEnum sideLookingAt);

        void updateBlock(BlockArrayData* data);

        bool activateBlock(World* world, ItemStack* stack);

        bool isBlockAir();

    private:
        BlockPos pos;
        std::shared_ptr<Block> type;
        int data = 0;
        bool isAir = false;
};
#endif