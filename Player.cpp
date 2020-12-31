#include "Player.h"
#include "World.h"
#include <iostream>
#include "Events.h"

Player::Player(World* _world) : pos(Pos(0, 5, 0)), world(_world) {
    world->getTimerMap()->addTimerToMap("playerUpdateTimer");
}

void Player::updatePlayerInWorld(World* world) {
    long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(world->getTimerMap()->getTimerDurationAndReset("playerUpdateTimer")).count();

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
    //std::cout << "x: " << pos.x << ", y: " << pos.y << ", z: " << pos.z << std::endl;
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
    return AABB(0, 0, 0, 0.5, 2, 0.5);
}

RenderedModel Player::getRenderedModel() {
    RenderedPoint p1 = RenderedPoint(0, 0, 0);
    RenderedPoint p2 = RenderedPoint(0.5, 0, 0);
    RenderedPoint p3 = RenderedPoint(0.5, 0, 0.5);
    RenderedPoint p4 = RenderedPoint(0, 0, 0.5);
    RenderedPoint p5 = RenderedPoint(0, 2, 0);
    RenderedPoint p6 = RenderedPoint(0.5, 2, 0);
    RenderedPoint p7 = RenderedPoint(0.5, 2, 0.5);
    RenderedPoint p8 = RenderedPoint(0, 2, 0.5);

    RenderedTriangle t1 = RenderedTriangle(p1, p2, p3);
    RenderedTriangle t2 = RenderedTriangle(p1, p4, p3);
    RenderedTriangle t3 = RenderedTriangle(p5, p6, p7);
    RenderedTriangle t4 = RenderedTriangle(p5, p8, p7);
    RenderedTriangle t5 = RenderedTriangle(p1, p4, p8);
    RenderedTriangle t6 = RenderedTriangle(p1, p5, p8);
    RenderedTriangle t7 = RenderedTriangle(p2, p3, p7);
    RenderedTriangle t8 = RenderedTriangle(p2, p6, p7);
    RenderedTriangle t9 = RenderedTriangle(p1, p2, p5);
    RenderedTriangle t10 = RenderedTriangle(p2, p5, p6);
    RenderedTriangle t11 = RenderedTriangle(p3, p4, p8);
    RenderedTriangle t12 = RenderedTriangle(p3, p7, p8);

    RenderedTriangle triangleArray[12] = {t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12};
    std::vector<RenderedTriangle> triangles = std::vector<RenderedTriangle>();

    for(RenderedTriangle triangle : triangleArray) {
        triangles.push_back(triangle);
    }

    return RenderedModel(triangles);
}

Pos Player::getPos() {
    return pos;
}