#ifndef MODELREGISTER_H
#define MODELREGISTER_H

#include "BlockRenderedModel.h"
#include <vector>
#include <map>
#include <tuple>
#include <memory>
#include "Block.h"
#include "TextureArrayCreator.h"

class ModelRegister {
    public:
        ModelRegister();

        void registerModel(TextureArrayCreator* texCreator, std::shared_ptr<Block> block, int metadata);

        std::tuple<unsigned int, unsigned int> getVAOAndVBO(std::shared_ptr<Block> block, int metadata);

    private:
        std::map<std::string, std::tuple<unsigned int, unsigned int>> blockToVAOAndVBOMap = std::map<std::string, std::tuple<unsigned int, unsigned int>>();
};

#endif
