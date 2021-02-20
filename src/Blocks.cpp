#include "Blocks.h"

const std::shared_ptr<Block> Blocks::dirt = std::make_shared<BlockDirt>();
const std::shared_ptr<Block> Blocks::cobblestone = std::make_shared<BlockCobblestone>();
const std::shared_ptr<Block> Blocks::grass = std::make_shared<BlockGrass>();

void Blocks::initTextureArrayCreator(TextureArrayCreator* texCreator) {
    for(int i = 0; i < 6; ++i) {
        if(Blocks::dirt == nullptr) {
            abort();
        }
        texCreator->addTextureToList(Blocks::dirt->getTextureName(SideEnum(i)));
        texCreator->addTextureToList(Blocks::cobblestone->getTextureName(SideEnum(i)));
        texCreator->addTextureToList(Blocks::grass->getTextureName(SideEnum(i)));
    }
}