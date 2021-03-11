#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include "Model.h"
#include <memory>
#include <iostream>
#include "BlockPos.h"
#include "SideEnum.h"
#include "BlockRenderedModel.h"

class Block {
    public:
        Block();

        virtual std::string getName();

        AABB getAABB(int data = 0);

        virtual BlockRenderedModel getRenderedModel(int data = 0);

        virtual std::string getTextureName(SideEnum side, int data = 0);

        virtual bool isSolid(int data = 0);

        virtual bool isOpaque(int data = 0);

        virtual int getXRotation(int data);

        virtual int getYRotation(int data);

        virtual int getZRotation(int data);

        virtual void onPlaced(SideEnum side, int* data);

        virtual ~Block() = default;
};
#endif