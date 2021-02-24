#include "Blocks.h"

const std::shared_ptr<Block> Blocks::dirt = std::make_shared<BlockDirt>();
const std::shared_ptr<Block> Blocks::cobblestone = std::make_shared<BlockCobblestone>();
const std::shared_ptr<Block> Blocks::grass = std::make_shared<BlockGrass>();
const std::shared_ptr<Block> Blocks::log = std::make_shared<BlockLog>();
const std::shared_ptr<Block> Blocks::leaf = std::make_shared<BlockLeaf>();
const std::shared_ptr<Block> Blocks::water = std::make_shared<BlockWater>();

void Blocks::initTextureArrayCreator(TextureArrayCreator* texCreator) {
    for(int i = 0; i < 6; ++i) {
        texCreator->addTextureToList(Blocks::dirt->getTextureName(SideEnum(i)));
        texCreator->addTextureToList(Blocks::cobblestone->getTextureName(SideEnum(i)));
        texCreator->addTextureToList(Blocks::grass->getTextureName(SideEnum(i)));
        texCreator->addTextureToList(Blocks::log->getTextureName(SideEnum(i)));
        texCreator->addTextureToList(Blocks::leaf->getTextureName(SideEnum(i)));
        texCreator->addTextureToList(Blocks::water->getTextureName(SideEnum(i)));
    }
}