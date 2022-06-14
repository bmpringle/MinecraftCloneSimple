#ifndef BLOCKPOS_H
#define BLOCKPOS_H

#include <string>

class BlockPos {
    public:
        int x = 0;
        int y = 0;
        int z = 0;

        BlockPos getAbove();

        BlockPos getBelow();

        BlockPos getLeft();

        BlockPos getRight();

        BlockPos getFront();

        BlockPos getBehind();

        BlockPos(int _x, int _y, int _z);

        BlockPos operator+(BlockPos rhs);

        bool operator==(BlockPos rhs);

        bool operator!=(BlockPos rhs);

        friend bool operator<(BlockPos lhs, BlockPos rhs);

        void print() const;

        std::string to_string() const; 
};

#endif
