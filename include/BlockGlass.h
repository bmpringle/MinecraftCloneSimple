#ifndef BLOCKGLASS_H
#define BLOCKGLASS_H

#include "Block.h"

class BlockGlass : public Block {
    public:
        BlockGlass();
        
        std::string getName() override;

        std::string getTextureName(SideEnum side, int data) override;  

        bool isOpaque(int data = 0) override;

        BlockRenderedModel getRenderedModel(int data = 0) override;
};
#endif


/*
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
*/