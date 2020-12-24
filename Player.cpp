#include "Player.h"
#include "World.h"
#include <iostream>
#include "Events.h"

Player::Player(World* _world) : pos(Pos(0, 5, 0)), world(_world) {

}

void Player::updatePlayerInWorld(World* world) {
    long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - previousUpdate).count();

    currentYSpeed -= (world->getWorldGravity() / 1000.0 * milliseconds);

    currentYSpeed = (currentYSpeed < -5) ? -5 : currentYSpeed;

    pos.y += (currentYSpeed / 1000.0 * milliseconds);

    AABB playerAABB = getAABB();

    playerAABB.startX += pos.x;
    playerAABB.startY += pos.y;
    playerAABB.startZ += pos.z;

    for(int i = 0; i < world->getBlockData()->getRawBlockArray().size(); ++i) {
        std::shared_ptr<Block> block = world->getBlockData()->getRawBlockArray().at(i);
        AABB blockAABB = block->getAABB();
        blockAABB.startX += block->getPos().x;
        blockAABB.startY += block->getPos().y;
        blockAABB.startZ += block->getPos().z;

        bool colliding = AABBIntersectedByAABB(playerAABB, blockAABB);

        if(colliding) {
            pos.y -= (currentYSpeed / 1000.0 * milliseconds);
            currentYSpeed = 0;
            break;
        }
    }

    previousUpdate = std::chrono::system_clock::now();
    std::cout << "x: " << pos.x << ", y: " << pos.y << ", z: " << pos.z << std::endl;
}

void Player::listenTo(std::shared_ptr<Event> e) {
    Pos previousPos = pos;

    if(e->getEventID() == "KEYHELD") {
        KeyHeldEvent keyEvent = *dynamic_cast<KeyHeldEvent*>(e.get());

        long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(keyEvent.duration).count();
        
        if(!keyEvent.key) {
            return;
        }

        if(keyEvent.key == 'w') {
            pos.z += metersPerSecond / 1000.0 * milliseconds; 
        }

        if(keyEvent.key == 'a') {
            pos.x -= metersPerSecond / 1000.0 * milliseconds; 
        }

        if(keyEvent.key == 's') {
            pos.z -= metersPerSecond / 1000.0 * milliseconds; 
        }

        if(keyEvent.key == 'd') {
            pos.x += metersPerSecond / 1000.0 * milliseconds; 
        }
    }

    if(e->getEventID() == "KEYPRESSED") {
        KeyPressedEvent keyEvent = *dynamic_cast<KeyPressedEvent*>(e.get());

        if(keyEvent.key == ' ') {
            if(currentYSpeed == 0) {
                currentYSpeed = 2.2;
            }
        }
    }

    AABB playerAABB = getAABB();

    playerAABB.startX += pos.x;
    playerAABB.startY += pos.y;
    playerAABB.startZ += pos.z;

    for(int i = 0; i < world->getBlockData()->getRawBlockArray().size(); ++i) {
        std::shared_ptr<Block> block = world->getBlockData()->getRawBlockArray().at(i);
        AABB blockAABB = block->getAABB();
        blockAABB.startX += block->getPos().x;
        blockAABB.startY += block->getPos().y;
        blockAABB.startZ += block->getPos().z;

        bool colliding = AABBIntersectedByAABB(playerAABB, blockAABB);

        if(colliding) {
            pos = previousPos;
            break;
        }
    }
}

AABB Player::getAABB() {
    return AABB(0.25, 0, 0.25, 0.5, 2, 0.5);
}