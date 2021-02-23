#include "BlockLog.h"

BlockLog::BlockLog() {

}

std::string BlockLog::getTextureName(SideEnum side) {
    switch(side) {
        case UP:
            return "log_end.png";
        case DOWN:
            return "log_end.png";
        default:
            return "log_side.png";
    }
}

std::string BlockLog::getName() {
    return "log";
}