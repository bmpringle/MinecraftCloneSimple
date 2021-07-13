#ifndef MODELREGISTER_H
#define MODELREGISTER_H

#include "BlockRenderedModel.h"
#include <vector>
#include <map>

class ModelRegister {
    public:
        ModelRegister();

        bool hasID(int id);

        int registerModel(std::vector<float>& vertexBuffer, std::vector<int>& indices);

        std::vector<float>& getVertexBuffer();

        std::vector<int>& getIndices(int id);

    private:

        //Combined VBO
        std::vector<float> vertexBuffer;

        //id --> indices map
        std::map<int, std::vector<int>> idIndicesMap;

        //id to be used on next registered model
        int nextID;
};

#endif
