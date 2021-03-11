#include "BlockLog.h"

BlockLog::BlockLog() {

}

std::string BlockLog::getTextureName(SideEnum side, int data = 0) {
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

int BlockLog::getXRotation(int data) {
    if(data == 0) {
        return 0;
    }

    if(data == 1) {
        return 0;
    }

    if(data == 2) {
        return 90;
    }
    return 0;
}

int BlockLog::getYRotation(int data) {
    return 0;
}

int BlockLog::getZRotation(int data) {
    if(data == 0) {
        return 0;
    }

    if(data == 1) {
        return 90;
    }

    if(data == 2) {
        return 0;
    }
    return 0;
}

void BlockLog::onPlaced(SideEnum side, int* data) {
    switch(side) {
        case UP:
            *data = 0;
            return;
        case DOWN:
            *data = 0;
            return;
        case NORTH:
            *data = 2;
            return;
        case SOUTH:
            *data = 2;
            return;
        case EAST:
            *data = 1;
            return;
        case WEST:
            *data = 1;
            return;
    }
    return;
}