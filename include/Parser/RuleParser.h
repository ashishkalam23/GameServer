#include <vector>
#include <string>
#include <variant>
#include <unordered_map>
#include <iostream>
#include <any>
#include <State/Game.h>
#include <State/Player.h>


//these classes will implement the identifiers in each of the section
class ControlExecuter;
class ListExecuter;
class TimingExecuter;
class InputExecuter;
class OutputExecuter;
class ExpressionExecuter;

class RuleParser{

    public:

    enum class RuleType
    {
        CONTROL,
        LIST_OPERATION,
        TIMING,
        INPUT,
        OUTPUT,
        VAR_EXPRESSION,
        UNKNOWN
    };

    RuleParser(std::vector<std::string>);

    //to execute all lines in the rules section
    void executeTurn();

    //to execute a single line in the rules section
    //the string will probably be changed to a class that holds all nodes of the parsed game file
    void executeLine(RuleType,std::string);

    RuleType getRuleType(std::string);

    void addVariable(std::string, std::any);

    std::unordered_map<std::string, std::any> getVariables();
    private:

        std::vector<std::string> rulesData;
    
        //initialize variables based off control structures
        //thought about using variants but a lot of possible types 
        std::unordered_map<std::string, std::any> variables;

        //create 
        // ControlExecuter controlExecuter;
        // ListExecuter listExecuter;
        // TimingExecuter timingExecuter;
        // InputExecuter inputExecuter;
        // OutputExecuter outputExecuter;
        // ExpressionExecuter expressionExecuter;

};



//need to implement the functions for each of the classes
//these all take in a Rule object but can be replace with Game object with a variables vector 

// class ControlExecuter{
//     public:
//     ControlExecuter(Rule&);
//     void chooseFunction(std::string);
//     //used for for, while, parallel for, in parallel, match
// };

// class ListExecuter{
//     public:
//     ListExecuter(Rule&);
//     void chooseFunction(std::string);
//     //used for extend,reverse,shuffle,sort,deal,discard
// };

// class TimingExecuter{
//     public:
//     TimingExecuter(Rule&);
//     void chooseFunction(std::string);
//     //used for timer
// };

// class InputExecuter{
//     public:
//     InputExecuter(Rule&);
//     void chooseFunction(std::string);
//     //used for input
// };

// class OutputExecuter{
//     public:
//     OutputExecuter(Rule&);
//     void chooseFunction(std::string);
//     //used for Message, Scores
// };

// class ExpressionExecuter{
//     public:
//     ExpressionExecuter(Rule&);
//     void chooseFunction(std::string);
//     //used for Assignment (<-), num attributes (count.upfrom(1)), list attributes
// };