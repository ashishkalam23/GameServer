#include <cpp-tree-sitter.h>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <optional>
#include <variant>
#include <unordered_map>
#include <iostream>

//for optional data, use has_value to check if it has a value
struct SetupData{
    std::string name;
    std::string kind;  
    std::string prompt;
    std::optional<std::pair<int,int>> range;
    // std::optional<std::map<std::string,std::string>> choices;

    //this will have to be changed later
    //grammar says default value can be  bool,num,string,list,map, or identifier?
    std::optional<std::variant<bool,int,std::string>> defaultValue;

};

struct Config{
    std::string name;
    std::pair<int,int> playerRange;
    bool hasAudience;
    std::vector<SetupData> settings;
};


struct Value;

using Literal = std::variant<int,std::string,bool,double,float>;
using List = std::vector<Value>;
using Map = std::unordered_map<std::string,Literal>;


struct Value {
    std::variant<Literal,List,Map> val;
};


struct Printer{
    void operator()(const int& i) const {
        std::cout << i << std::endl;
    }

    void operator()(const std::string& s) const {
        std::cout << s << std::endl;
    }

    void operator()(const bool& b) const {
        std::cout << b << std::endl;
    }

    void operator()(const double& d) const {
        std::cout << d << std::endl;
    }

    void operator()(const float& f) const {
        std::cout << f << std::endl;
    }

    void operator()(const List& l) const {
        for(auto& val : l){
            std::visit(Printer{},val.val);
        }
    }
    void operator()(const Literal& l) const {
        std::visit(Printer{},l);
    }
    //operator for list of map values


    void operator()(const Map& m) const {
        for(auto& [key,val] : m){
            std::cout << key << ": ";
            std::visit(Printer{},val);
        }
        std::cout << "\n";
    }
    
};


class StateVariables{
    public:
        void addData(const std::string& section,const std::string& name, Value value);
        Value& getVariable(const std::string& section,const std::string& name);

        void printState();

        std::unordered_map<std::string, Value> getConstants();
        std::unordered_map<std::string, Value> getVariables();
        std::unordered_map<std::string, Value> getPer_player();
        std::unordered_map<std::string, Value> getPer_audience();

    private:
        std::unordered_map<std::string, Value> constants;
        std::unordered_map<std::string, Value> variables;
        std::unordered_map<std::string, Value> per_player;
        std::unordered_map<std::string, Value> per_audience;

};

struct ParserObjects{
    Config config;
    StateVariables state;
};


class TreeSitter{
    public:

        //initialize the parser
        TreeSitter(std::string filename);
        void fileToString(std::string filename);
        std::string getSourceString();

        //config parsing
        void parseConfig(ts::Node root);
        void createConfig();
        void parseSetup(ts::Node setupNode);
        void createSetting(ts::Node setting);

        //helper functions for getting data from nodes
        std::string getStringText(ts::Node);
        bool stringToBool(std::string hasAudience);
        std::pair<int,int> parsePlayerRange(std::string playerRange);

        //runs all code to return objects from parser
        void initalize();

        //print functions for testing
        void printConfig();
        void printState();

        //state parsing
        void parseState(ts::Node stateNode);
        void addState(std::string section, std::string key,ts::Node stateNode);
        Literal typeConversion(ts::Node valueNode);
        Map handleMap(ts::Node mapNode);

        //getters
        Config getConfig();
        ParserObjects getParserObjects();

    
    private:
        std::string filename;
        ts::Language language;
        ts::Parser parser;
        std::map<std::string,std::string> data;
        std::string sourceString;
        Config config;
        StateVariables state;
};

