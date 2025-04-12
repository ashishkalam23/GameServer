#include "Parser/GameData.h"
#include "Parser/TreeSitter.h"
#include <iostream>

//TreeSitter executable takes in a filename as an argument
//I have the rock-paper-scissors and corpus folder from Nick's socialgaming repo
//in my build directory for testing         ex: bin/treesitter rock-paper-scissors.game


GameData::GameData(std::map<std::string,std::string>& data) : data(data){}


std::string_view GameData::getConfig() const {
    auto it = data.find("configuration");
    return it != data.end() ? std::string_view(it->second) : std::string_view("");
}

std::string_view GameData::getConstants() const {
    auto it = data.find("constants");
    return it != data.end() ? std::string_view(it->second) : std::string_view("");
}

std::string_view GameData::getPerAudience() const {
    auto it = data.find("per_audience");
    return it != data.end() ? std::string_view(it->second) : std::string_view("");
}

std::string_view GameData::getPerPlayer() const {
    auto it = data.find("per_player");
    return it != data.end() ? std::string_view(it->second) : std::string_view("");
}

std::string_view GameData::getRules() const {
    auto it = data.find("rules");
    return it != data.end() ? std::string_view(it->second) : std::string_view("");
}

std::string_view GameData::getVariables() const {
    auto it = data.find("variables");
    return it != data.end() ? std::string_view(it->second) : std::string_view("");
}


class configParser : public dataParser{
    private:
        void parseImpl(std::string_view data) const override{
            std::cout << "Create config class after parsing: " << std::endl;
        }
};

class constantsParser : public dataParser{
    private:
        void parseImpl(std::string_view data) const override{
            std::cout << "Create constants object after parsing: " << std::endl;
        }
};

class perAudienceParser : public dataParser{
    private:
        void parseImpl(std::string_view data) const override{
            std::cout << "Create perAudience object after parsing: " << std::endl;
        }
};

class perPlayerParser : public dataParser{
    private:
        void parseImpl(std::string_view data) const override{
            std::cout << "Create perPlayer object after parsing: " << std::endl;
        }
};

class rulesParser : public dataParser{
    private:
        void parseImpl(std::string_view data) const override{
            std::cout << "Create rules object after parsing: " << std::endl;
        }
};

class variablesParser : public dataParser{
    private:
        void parseImpl(std::string_view data) const override{
            std::cout << "Create variables object after parsing: " << std::endl;
        }
};

//each class will have a dataParser object that will parse the data and get whats needed


// int main(int argc, char* argv[]){
//     TreeSitter a;

//     if (argc < 2) {
//         std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
//         return 1;
//     }
  
//     a.fileToString(argv[1]);
//     // a.fileToString("rock-paper-scissors.game");
//     std::map<std::string,std::string> treeData = a.mapData();
//     GameData b(treeData);
//     std::cout << b.getConfig() << "\n" << b.getConstants() << "\n"<< b.getPerAudience() << "\n"<< b.getPerPlayer() << "\n"<< b.getRules() << "\n"
//     << b.getVariables() << "\n";

// }