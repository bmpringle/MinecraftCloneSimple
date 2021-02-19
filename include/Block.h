#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include "Model.h"
#include <memory>
#include <iostream>
#include "BlockPos.h"


class Block : public Model {
    public:
        Block();

        virtual std::string getName();

        AABB getAABB();

        RenderedModel getRenderedModel();

        virtual std::string getTextureName();

        virtual ~Block() = default;
};
#endif