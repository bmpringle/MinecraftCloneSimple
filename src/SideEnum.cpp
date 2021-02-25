#include "SideEnum.h"
#include <string>

std::string toString(SideEnum side) {
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
    }
}