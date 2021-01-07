#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include "Model.h"

struct BlockPos {
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
};

class Block : public Model {
    public:
        Block();

        std::string getName();

        BlockPos getPos();

        void setPos(BlockPos pos_);

        AABB getAABB();

        RenderedModel getRenderedModel();

        virtual std::string getTextureName();
    private:
        BlockPos pos;
};
#endif