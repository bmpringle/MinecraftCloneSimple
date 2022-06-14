#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include "Model.h"
#include <memory>
#include <iostream>
#include "BlockPos.h"
#include "SideEnum.h"
#include "BlockRenderedModel.h"

class World;
class BlockArrayData;
class BlockData;
class ItemStack;

class Block {
    public:
        Block();

        virtual std::string getName();

        virtual AABB getAABB(int data = 0);

        virtual BlockRenderedModel getRenderedModel(int data = 0);

        virtual void rotateModel(BlockRenderedModel& model, int data);

        virtual std::string getTextureName(SideEnum side, int data = 0);

        virtual bool isSolid(int data = 0);

        virtual bool isOpaque(int data = 0);

        virtual bool isLiquid(int data = 0);

        virtual int getXRotation(int data);

        virtual int getYRotation(int data);

        virtual int getZRotation(int data);

        virtual void onPlaced(SideEnum hPlacementAngle, SideEnum sideLookingAt, int* data);

        virtual void updateBlock(BlockArrayData* data, BlockData* blockToUpdate);

        virtual bool onBlockActivated(World* world, BlockPos pos, ItemStack* stack, int* data);

        virtual int getNumberOfVariants();

        virtual ~Block() = default;
    protected:
        int numberOfVariants = 1;
        AABB aabb = AABB(0, 0, 0, 1, 1, 1);

};
#endif
