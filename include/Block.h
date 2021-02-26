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

        AABB getAABB();

        virtual BlockRenderedModel getRenderedModel();

        virtual std::string getTextureName(SideEnum side);

        virtual bool isSolid();

        virtual bool isOpaque();

        virtual ~Block() = default;
};
#endif