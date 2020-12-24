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
};

class Block : public Model {
    public:
        Block();

        std::string getName();

        BlockPos getPos();

        void setPos(BlockPos pos_);

        AABB getAABB();
        
    private:
        BlockPos pos;
};
#endif