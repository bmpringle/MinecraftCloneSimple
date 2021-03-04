#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

#include <map>

enum Setting {
    FOV,
    MOVE_FORWARD,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_BACK,
    INVENTORY,
    SPRINT,
    SNEAK,
    JUMP,
    ATTACK,
    USE,
    USE_SECOND, //(optional) second use key
    SLOT1,
    SLOT2,
    SLOT3,
    SLOT4,
    SLOT5,
    SLOT6,
    SLOT7,
    SLOT8,
    SLOT9,
    DUMMY_SETTING,
};

class GameSettings {
    public:
        std::string getSetting(Setting s) {
            return settings[s];
        }

        void setSetting(Setting s, std::string str) {
            settings[s] = str;
        }

        std::map<Setting, std::string> getSettingsMap() {
            return settings;
        }

        void initSettings() {
            settings[FOV] = "90";
            settings[MOVE_FORWARD] = "w";
            settings[MOVE_LEFT] = "a";
            settings[MOVE_RIGHT] = "d";
            settings[MOVE_BACK] = "s";
            settings[INVENTORY] = "e";
            settings[SPRINT] = "LEFT_SHIFT";
            settings[SNEAK] = "RIGHT_SHIFT";
            settings[JUMP] = " ";
            settings[ATTACK] = "LEFT_CLICK";
            settings[USE] = "RIGHT_CLICK";
            settings[USE_SECOND] = "u";
            settings[SLOT1] = "1";
            settings[SLOT2] = "2";
            settings[SLOT3] = "3";
            settings[SLOT4] = "4";
            settings[SLOT5] = "5";
            settings[SLOT6] = "6";
            settings[SLOT7] = "7";
            settings[SLOT8] = "8";
            settings[SLOT9] = "9";
        }

        static const std::string settingToString(Setting s) {
            switch(s) {
                case FOV:
                    return "FOV";
                case MOVE_FORWARD:
                    return "MOVE_FORWARD";
                case MOVE_BACK:
                    return "MOVE_BACK";
                case MOVE_LEFT:
                    return "MOVE_LEFT";
                case MOVE_RIGHT:
                    return "MOVE_RIGHT";
                case INVENTORY:
                    return "INVENTORY";
                case SPRINT:
                    return "SPRINT";
                case SNEAK:
                    return "SNEAK";
                case JUMP:
                    return "JUMP";
                case ATTACK:
                    return "ATTACK";
                case USE:
                    return "USE";
                case USE_SECOND:
                    return "USE_SECOND";
                case SLOT1:
                    return "SLOT1";
                case SLOT2:
                    return "SLOT2";
                case SLOT3:
                    return "SLOT3";
                case SLOT4:
                    return "SLOT4";
                case SLOT5:
                    return "SLOT5";
                case SLOT6:
                    return "SLOT6";
                case SLOT7:
                    return "SLOT7";
                case SLOT8:
                    return "SLOT8";
                case SLOT9:
                    return "SLOT9";
                default:
                    return "NO_ENUM_CONVERSION_DEFINED";
            }
        }

    private:
        std::map<Setting, std::string> settings = std::map<Setting, std::string>();
};

#endif