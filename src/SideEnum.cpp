#include "SideEnum.h"
#include <string>

std::string SideEnumHelper::toString(SideEnum side) {
    switch(side) {
        case UP:
            return "UP";
        case DOWN:
            return "DOWN";
        case NORTH:
            return "NORTH";
        case SOUTH:
            return "SOUTH";
        case EAST:
            return "EAST";
        case WEST:
            return "WEST";
        case NEUTRAL:
            return "NEUTRAL";
    }
}

SideEnum SideEnumHelper::getOppositeSide(SideEnum side) {
    switch(side) {
        case UP:
            return DOWN;
        case DOWN:
            return UP;
        case NORTH:
            return SOUTH;
        case SOUTH:
            return NORTH;
        case EAST:
            return WEST;
        case WEST:
            return EAST;
        case NEUTRAL:
            return NEUTRAL;
    }
}

const int SideEnumHelper::getEnumSize() {
    return 7;
}