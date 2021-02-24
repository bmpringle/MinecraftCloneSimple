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

        BlockRenderedModel getRenderedModel();

        virtual std::string getTextureName(SideEnum side);

        virtual bool isSolid();

        virtual ~Block() = default;
};
#endif