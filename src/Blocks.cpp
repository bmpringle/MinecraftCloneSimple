#include "Blocks.h"

#include "BlockDirt.h"
#include "BlockCobblestone.h"
#include "BlockGrass.h"
#include "BlockLog.h"
#include "BlockLeaf.h"
#include "BlockWater.h"
#include "BlockPlanks.h"
#include "BlockDoor.h"
#include "BlockGlass.h"

const std::shared_ptr<Block> Blocks::dirt = std::make_shared<BlockDirt>();
const std::shared_ptr<Block> Blocks::cobblestone = std::make_shared<BlockCobblestone>();
const std::shared_ptr<Block> Blocks::grass = std::make_shared<BlockGrass>();
const std::shared_ptr<Block> Blocks::log = std::make_shared<BlockLog>();
const std::shared_ptr<Block> Blocks::leaf = std::make_shared<BlockLeaf>();
const std::shared_ptr<Block> Blocks::water = std::make_shared<BlockWater>();
const std::shared_ptr<Block> Blocks::planks = std::make_shared<BlockPlanks>();
const std::shared_ptr<Block> Blocks::door = std::make_shared<BlockDoor>();
const std::shared_ptr<Block> Blocks::glass = std::make_shared<BlockGlass>();

std::map<std::string, std::shared_ptr<Block>> Blocks::blockMap = {
            {dirt->getName(), dirt},
            {cobblestone->getName(), cobblestone},
            {grass->getName(), grass},
            {log->getName(), log},
            {leaf->getName(), leaf},
            {water->getName(), water},
            {planks->getName(), planks},
            {door->getName(), door},
            {glass->getName(), glass}
};