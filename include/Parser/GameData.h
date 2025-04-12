#include <map>
#include <string>
#include <string_view>

class GameData{
    public:
        GameData(std::map<std::string,std::string>& data);
        std::string_view getConfig() const;
        std::string_view getConstants() const;
        std::string_view getPerAudience() const;
        std::string_view getPerPlayer() const;
        std::string_view getRules() const;
        std::string_view getVariables() const;

        
    private:
        std::map<std::string,std::string> data;
};

class dataParser{
    public:
        void parseData(std::string_view data) const{
            parseImpl(data);
        }
    private:
        virtual void parseImpl(std::string_view data) const = 0;

};