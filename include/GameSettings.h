#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

#include <map>
#include <unordered_map>

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
    SHOW_FPS,
    DUMMY_SETTING,
};

enum SettingType {
    BOOL,
    INTEGER,
    STRING,
    INPUT
};

class SettingState {
    public:
        virtual SettingType getSettingType() = 0;
        virtual std::string getSettingState() = 0;
        virtual void setSettingState(std::string state) = 0;
};

class SettingStateBool final : public SettingState {
    public:
        SettingStateBool(bool startingState) : state(startingState) {

        }

        SettingType getSettingType() override {
            return BOOL;
        }

        std::string getSettingState() override {
            if(state) {
                return "TRUE";
            }else {
                return "FALSE";
            }
        }

        void setSettingState(std::string state) override {
            if(state == "TRUE") {
                this->state = true; 
            }else if(state == "FALSE") {
                this->state = false;
            }
            throw std::invalid_argument("state must be TRUE or FALSE when calling setSettingState on a SettingStateBool object");
        }

    private:
        bool state;

};

class SettingStateString final : public SettingState {
    public:
        SettingStateString(std::string startingState) : state(startingState) {

        }

        SettingType getSettingType() override {
            return STRING;
        }

        std::string getSettingState() override {
            return state;
        }

        void setSettingState(std::string state) override {
            this->state = state;
        }

    private:
        std::string state;

};

class SettingStateInteger final : public SettingState {
    public:
        SettingStateInteger(int startingState) : state(startingState) {

        }

        SettingType getSettingType() override {
            return INTEGER;
        }

        std::string getSettingState() override {
            return std::to_string(state);
        }

        void setSettingState(std::string state) override {
            this->state = std::stoi(state);
        }
    private:
        int state;
};

class GameSettings {
    public:
        std::shared_ptr<SettingState> getSetting(Setting s) {
            return settings.at(s);
        }

        void setSetting(Setting s, std::string str) {
            try {
                if(settingToSettingType[s] == INTEGER) {
                    settings.at(s) = std::make_shared<SettingStateInteger>(std::stoi(str));
                }else if(settingToSettingType[s] == BOOL) {
                    settings.at(s) = std::make_shared<SettingStateBool>(str == "TRUE");
                }else if(settingToSettingType[s] == STRING || settingToSettingType[s] == INPUT) {
                    settings.at(s) = std::make_shared<SettingStateString>(str);
                }
            }catch(std::invalid_argument ex) {
                std::cout << str << " is an invalid setting for the setting " << settingToString(s) << std::endl;
            }
        }

        std::map<Setting, std::shared_ptr<SettingState>> getSettingsMap() {
            return settings;
        }

        void initDefaultSettings() {
            settings[FOV] = std::make_shared<SettingStateInteger>(90);
            settings[MOVE_FORWARD] = std::make_shared<SettingStateString>("w");
            settings[MOVE_LEFT] = std::make_shared<SettingStateString>("a");
            settings[MOVE_RIGHT] = std::make_shared<SettingStateString>("d");
            settings[MOVE_BACK] = std::make_shared<SettingStateString>("s");
            settings[INVENTORY] = std::make_shared<SettingStateString>("e");
            settings[SPRINT] = std::make_shared<SettingStateString>("LEFT_SHIFT");
            settings[SNEAK] = std::make_shared<SettingStateString>("RIGHT_SHIFT");
            settings[JUMP] = std::make_shared<SettingStateString>(" ");
            settings[ATTACK] = std::make_shared<SettingStateString>("LEFT_CLICK");
            settings[USE] = std::make_shared<SettingStateString>("RIGHT_CLICK");
            settings[USE_SECOND] = std::make_shared<SettingStateString>("u");
            settings[SLOT1] = std::make_shared<SettingStateString>("1");
            settings[SLOT2] = std::make_shared<SettingStateString>("2");
            settings[SLOT3] = std::make_shared<SettingStateString>("3");
            settings[SLOT4] = std::make_shared<SettingStateString>("4");
            settings[SLOT5] = std::make_shared<SettingStateString>("5");
            settings[SLOT6] = std::make_shared<SettingStateString>("6");
            settings[SLOT7] = std::make_shared<SettingStateString>("7");
            settings[SLOT8] = std::make_shared<SettingStateString>("8");
            settings[SLOT9] = std::make_shared<SettingStateString>("9");
            settings[SHOW_FPS] = std::make_shared<SettingStateBool>(true);
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
                case SHOW_FPS:
                    return "SHOW_FPS";
                default:
                    return "NO_ENUM_CONVERSION_DEFINED";
            }
        }

        static Setting stringToSetting(std::string s) {
            for(int setting = 0; setting < DUMMY_SETTING; ++setting) {

                std::string str = GameSettings::settingToString((Setting)setting);
                if(s == str) {
                    return (Setting)setting;
                }
            }
            return DUMMY_SETTING;         
        }

        void parseOptionsFromFile(std::string file) {
            std::ifstream fileStream = std::ifstream(file);
            std::string line = "";
            while(std::getline(fileStream, line)) {
                try {
                    std::pair<Setting, std::string> option = parseOptionFromString(line);
                    
                    if(settingToSettingType[option.first] == INTEGER) {
                        settings[option.first] = std::make_shared<SettingStateInteger>(std::stoi(option.second));
                    }else if(settingToSettingType[option.first] == BOOL) {
                        settings[option.first] = std::make_shared<SettingStateBool>(option.second == "TRUE");
                    }else if(settingToSettingType[option.first] == STRING || settingToSettingType[option.first] == INPUT) {
                        settings[option.first] = std::make_shared<SettingStateString>(option.second);
                    }
                    
                }catch(Setting s) {
                    std::cout << "options file corrupted. loading defaults." << std::endl;
                    initDefaultSettings();
                    break;
                }catch(std::invalid_argument ex) {
                    std::cout << "options file corrupted. loading defaults." << std::endl;
                    initDefaultSettings();
                    break;
                }
            }
        }

        void saveOptionsToFile(std::string file) {
            std::string encodedOptions = "";
            for(auto& option : settings) {
                encodedOptions += settingToString(option.first) + ": \"" + option.second->getSettingState() + "\"\n";
            }
            std::ofstream fileStream = std::ofstream(file);
            fileStream << encodedOptions;
        }

        static SettingType getSettingType(Setting s) {
            return settingToSettingType[s];
        }

    private:
        std::map<Setting, std::shared_ptr<SettingState> > settings = std::map<Setting, std::shared_ptr<SettingState> >();

        static inline std::unordered_map<Setting, SettingType> settingToSettingType = {
            {FOV, INTEGER},
            {MOVE_FORWARD, INPUT},
            {MOVE_LEFT, INPUT},
            {MOVE_RIGHT, INPUT},
            {MOVE_BACK, INPUT},
            {INVENTORY, INPUT},
            {SPRINT, INPUT},
            {SNEAK, INPUT},
            {JUMP, INPUT},
            {ATTACK, INPUT},
            {USE, INPUT},
            {USE_SECOND, INPUT}, //(optional) second use key
            {SLOT1, INPUT},
            {SLOT2, INPUT},
            {SLOT3, INPUT},
            {SLOT4, INPUT},
            {SLOT5, INPUT},
            {SLOT6, INPUT},
            {SLOT7, INPUT},
            {SLOT8, INPUT},
            {SLOT9, INPUT},
            {SHOW_FPS, BOOL},
            {DUMMY_SETTING, STRING}
        };

        static const std::pair<Setting, std::string> parseOptionFromString(std::string s) {
            
            Setting setting = DUMMY_SETTING;
            std::string value = "";

            //0 is parsing setting, 1 is parsing space in between the two, and 2 is parsing value 
            int partOfLineParse = 0;
            
            std::string parseBuffer = "";

            for(char c : s) {
                if(c == ':') {
                    partOfLineParse = 1;
                    setting = GameSettings::stringToSetting(parseBuffer);
                    parseBuffer = "";
                }else {
                    if(partOfLineParse == 0) {
                        parseBuffer += c;
                    }else if(partOfLineParse == 1) {
                        if(c == '"') {
                            partOfLineParse = 2;
                        }
                    }else {
                        if(c == '"') {
                            break;
                        }else {
                            value += c;
                        }
                    }
                }
            }

            if(setting == DUMMY_SETTING) {
                throw DUMMY_SETTING;
            }else {
                return std::pair<Setting, std::string>(setting, value);
            }
        }
};

#endif