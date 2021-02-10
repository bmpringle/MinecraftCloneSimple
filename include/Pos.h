#ifndef POS_H
#define POS_H

#include "BlockPos.h"

class Pos {
    public:
        Pos(float _x, float _y, float _z);

        float x = 0;
        float y = 0;
        float z = 0;

        Pos operator+(Pos rhs);

        BlockPos toBlockPos();
};
#endif