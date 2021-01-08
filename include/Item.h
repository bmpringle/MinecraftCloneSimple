#ifndef ITEM_H
#define ITEM_H

#include <string>

class World;

class Item {
    public:
        virtual std::string getItemName();

        virtual void onRightClick(World* world);
    private:
};
#endif