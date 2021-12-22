#ifndef SBDA_H
#define SBDA_H

#include <array>
#include "BinaryTree/Serializable.h"
#include <string>
#include "BlockData.h"
#include "Blocks.h"
#include <algorithm>

class SBDA final : public BinarySerializable {
    public:
        void serialize(std::string* strData, int depth) override {
            strData->append("[");
            for(int i = 0; i < array.size(); ++i) {
                BlockData& blockData = array[i];
                if(!blockData.isBlockAir()) {
                    BlockPos pos = blockData.getPos();
                    strData->append("(" + blockData.getBlockType()->getName() + "|" + std::to_string(pos.x) + "|" + std::to_string(pos.y) + "|" + std::to_string(pos.z) + "|" + std::to_string(blockData.getData()) + ")");
                    strData->append(",");
                }
            }
            strData->pop_back();
            strData->append("]");
        }

        void deserialize(std::string* data, int depth) override {
            std::string lineData = data->substr(0, data->find_first_of(']') + 1);
            data->erase(0, data->find_first_of(']') + 1);
            lineData.erase(remove(lineData.begin(), lineData.end(), '['), lineData.end());
            lineData.erase(remove(lineData.begin(), lineData.end(), ']'), lineData.end());

            std::stringstream s2(lineData);
            while(std::getline(s2, lineData, ',')) {
                std::string rawData = lineData.substr(1, lineData.size() - 2);
                std::stringstream s3(rawData);
                std::string datatoken;
                //0 is blocktype, 1 is pos.x, 2 is pos.y, 3 is po.z
                int tokennum = 0;

                std::string typenameblock = "";
                std::string xstr = "";
                std::string ystr = "";
                std::string zstr = "";
                std::string datStr = "";

                while(std::getline(s3, datatoken, '|')) {
                    switch(tokennum) {
                        case 0:
                            typenameblock = datatoken;
                            break;
                        case 1:
                            xstr = datatoken;
                            break;
                        case 2:
                            ystr = datatoken;
                            break;
                        case 3:
                            zstr = datatoken;
                            break;
                        case 4:
                            datStr = datatoken;
                    }
                    ++tokennum;
                }

                std::shared_ptr<Block> type = Blocks::blockMap[typenameblock];
                int x = std::stoi(xstr);
                int y = std::stoi(ystr);
                int z = std::stoi(zstr);
                int data = std::stoi(datStr);
                array[y] = BlockData(type, BlockPos(x, y, z));
                array[y].setData(data);
            }
        }

        BlockData& operator[](int index) {
            return array[index];
        }

        std::array<BlockData, 256> array = std::array<BlockData, 256>();
};
#endif