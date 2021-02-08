#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include "Model.h"
#include <memory>
#include <iostream>

class BlockPos {
    public:
        BlockPos(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {

        }

        int x = 0;
        int y = 0;
        int z = 0;

        friend BlockPos operator+(BlockPos lhs, BlockPos rhs) {
            BlockPos res = BlockPos(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
            return res;
        }

        friend bool operator==(BlockPos lhs, BlockPos rhs) {
            return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
        }

        friend bool operator!=(BlockPos lhs, BlockPos rhs) {
            return !((lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z));
        }

        void print() {
            std::cout << "pos: " << x << " " << y << " " << z << std::endl;
        }
};

class Block : public Model {
    public:
        Block();

        virtual std::string getName();

        BlockPos getPos();

        void setPos(BlockPos pos_);

        virtual std::shared_ptr<Block> create();

        AABB getAABB();

        RenderedModel getRenderedModel();

        virtual std::string getTextureName();

        virtual ~Block() = default;
    private:
        BlockPos pos;
};
#endif