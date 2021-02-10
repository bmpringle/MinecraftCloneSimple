#ifndef BLOCKPOS_H
#define BLOCKPOS_H

class BlockPos {
    public:
        int x = 0;
        int y = 0;
        int z = 0;

        BlockPos(int _x, int _y, int _z);

        BlockPos operator+(BlockPos rhs);

        bool operator==(BlockPos rhs);

        bool operator!=(BlockPos rhs);

        void print();
};

#endif