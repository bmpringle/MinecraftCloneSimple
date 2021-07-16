#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class World;

class Player final : public Listener, public Entity {
    public:
        Player(World* _world);
        void updateEntity(World* world) override;
        void listenTo(std::shared_ptr<Event> e) override;
        double getXRotation();
        double getYRotation();
        Pos getCameraPosition();
        Pos getCameraNormal();
        BlockPos* getBlockLookingAt();
        SideEnum getSideOfBlockLookingAt();
       
        void displayGui(Renderer* renderer);
        SideEnum horizontalSidePlacedOn();
        SideEnum sideLookingAt();

        bool isThirdPerson();

    private:
        void updatePlayerLookingAt(World* world);
        float raycast(AABB box, SideEnum* side, Pos cameraPosition, Pos cameraNormal);
        void updateHorizontalMotion();

        void processInput(std::string event, std::string key, std::shared_ptr<Event> e);

        World* world;
        GameSettings* settings;

        //in m/s
        float speed = 4.3;
        

        float yaw = 0;
        float pitch = 0;

        BlockPos* blockLookingAt = nullptr;

        //this exists to prevent the pointer you should use from going stale DO NOT READ THIS
        BlockPos internalBlockLookingAt = BlockPos(0, 0, 0);

        SideEnum sideOfBlockLookingAt = NORTH;

        int xInputDirection = 0;
        int zInputDirection = 0;

        std::unique_ptr<Gui> gui = nullptr; 

        int mouseX = 0;
        int mouseY = 0;

        bool thirdPerson = false;
};
#endif