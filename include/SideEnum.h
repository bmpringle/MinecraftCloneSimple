#ifndef SIDEENUM_H
#define SIDEENUM_H

#include <string>

enum SideEnum {
    DOWN,
    UP,
    NORTH,
    SOUTH,
    EAST,
    WEST,
    NEUTRAL
};

namespace SideEnumHelper {
    std::string toString(SideEnum side);
}

#endif