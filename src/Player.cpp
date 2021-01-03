#include "Player.h"
#include "World.h"
#include <iostream>
#include "Events.h"
#include <math.h>

Player::Player(World* _world) : pos(Pos(0, 5, 0)), world(_world) {
    world->getTimerMap()->addTimerToMap("playerUpdateTimer");
}

void Player::updatePlayerInWorld(World* world) {  
    //handle space bar and gravity from events
    long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(world->getTimerMap()->getTimerDurationAndReset("playerUpdateTimer")).count();

    currentYSpeed -= (world->getWorldGravity() / 1000.0 * milliseconds);

    currentYSpeed = (currentYSpeed < -78.4) ? -78.4 : currentYSpeed;

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

    Pos previousPos = pos;

    //handle WASD movement input grabbed during events
    double duration = moveVector.durationInMilliseconds;

    float unitX = 0;
    float unitZ = 0;

    if(moveVector.relativeZ > 0) {
        if(moveVector.relativeX > 0) {
            unitX = sqrt(2)/2;
            unitZ = sqrt(2)/2;
        }

        if(moveVector.relativeX == 0) {
            unitZ = 1;
        }

        if(moveVector.relativeX < 0) {
            unitX = -sqrt(2)/2;
            unitZ = sqrt(2)/2;
        }
    }

    if(moveVector.relativeZ == 0) {
        if(moveVector.relativeX > 0) {
            unitX = 1;
        }

        if(moveVector.relativeX == 0) {
            //dont move at all
        }

        if(moveVector.relativeX < 0) {
            unitX = -1;
        }
    }

    if(moveVector.relativeZ < 0) {
        if(moveVector.relativeX > 0) {
            unitX = sqrt(2)/2;
            unitZ = -sqrt(2)/2;
        }

        if(moveVector.relativeX == 0) {
            unitZ = -1;
        }

        if(moveVector.relativeX < 0) {
            unitX = -sqrt(2)/2;
            unitZ = -sqrt(2)/2;            
        }
    }

    double xMovRel = duration / 1000.0 * speed * unitX;
    double zMovRel = duration / 1000.0 * speed * unitZ;

    double degRADS = yaw/180.0*M_PI;
    double xMovAbs = xMovRel * cos(degRADS) - zMovRel * sin(degRADS);
    double zMovAbs = zMovRel * cos(degRADS) + xMovRel * sin(degRADS);

    pos.x += xMovAbs;
    pos.z += zMovAbs;

    moveVector.durationInMilliseconds = 0;
    moveVector.relativeX = 0;
    moveVector.relativeZ = 0;


    //handle collision
    playerAABB = getAABB();

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
    //std::cout << "x: " << pos.x << ", y: " << pos.y << ", z: " << pos.z << std::endl;
}

void Player::listenTo(std::shared_ptr<Event> e) {

    if(e->getEventID() == "KEYPRESSED") {
        KeyPressedEvent keyEvent = *dynamic_cast<KeyPressedEvent*>(e.get());

        if(keyEvent.key == ' ') {
            if(currentYSpeed == 0) {
                currentYSpeed = 10;
            }
        }
    }

    if(e->getEventID() == "KEYHELD") {
        KeyHeldEvent keyEvent = *dynamic_cast<KeyHeldEvent*>(e.get());

        long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(keyEvent.duration).count();

        if(!keyEvent.key) {
            return;
        }

        if(keyEvent.key == 'w') {
            if(moveVector.durationInMilliseconds == 0) {
                moveVector.durationInMilliseconds = milliseconds;
            }
            ++moveVector.relativeZ;
        }

        if(keyEvent.key == 'a') {
            if(moveVector.durationInMilliseconds == 0) {
                moveVector.durationInMilliseconds = milliseconds;
            }
            --moveVector.relativeX;
        }

        if(keyEvent.key == 's') {
            if(moveVector.durationInMilliseconds == 0) {
                moveVector.durationInMilliseconds = milliseconds;
            }
            --moveVector.relativeZ;
        }

        if(keyEvent.key == 'd') {
            if(moveVector.durationInMilliseconds == 0) {
                moveVector.durationInMilliseconds = milliseconds;
            }
            ++moveVector.relativeX;
        }
    }

    if(e->getEventID() == "MOUSEMOVED") {
        MouseMovedEvent mouseEvent = *dynamic_cast<MouseMovedEvent*>(e.get());
        yaw -= mouseEvent.xOffset;
        pitch -= mouseEvent.yOffset;

        if(pitch > 89) {
            pitch = 89;
        }
        if(pitch < -89) {
            pitch = -89;
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

double Player::getXRotation() {
    return yaw;
}
double Player::getYRotation() {
    return pitch;
}