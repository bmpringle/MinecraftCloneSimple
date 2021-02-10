#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "AABB.h"
#include "RenderedModel.h"

class Model {
    public:
        virtual AABB getAABB() = 0;

        virtual RenderedModel getRenderedModel() = 0;
};

#endif